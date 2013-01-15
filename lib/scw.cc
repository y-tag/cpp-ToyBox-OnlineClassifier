#include "scw.h"

#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include <algorithm>
#include <vector>
#include <utility>

#include "datum.h"
#include "serializer.h"
#include "deserializer.h"

namespace toybox {
namespace online {

SCW::SCW()
  : scw_type_(SCW1), feature_bit_(0), C_(0.0),
    phi_(0.0), psi_(0.0), xi_(0.0),
    mu_array_(NULL), sigma_array_(NULL) {
}

SCW::SCW(SCWType type, int feature_bit, double phi, double C)
  : scw_type_(type), feature_bit_(feature_bit), C_(C),
    phi_(phi), psi_(0.0), xi_(0.0),
    mu_array_(NULL), sigma_array_(NULL) {

    psi_ = 1.0 + (phi_ * phi_) / 2.0;
    xi_  = 1.0 + (phi_ * phi_);
    int feature_num = 1 << feature_bit;
    mu_array_    = new double[feature_num];
    sigma_array_ = new double[feature_num];
    std::fill_n(mu_array_,    feature_num, 0.0);
    std::fill_n(sigma_array_, feature_num, 1.0);
}

SCW::~SCW() {
  if (mu_array_ != NULL) {
    delete[] mu_array_; mu_array_ = NULL;
  }
  if (sigma_array_ != NULL) {
    delete[] sigma_array_; sigma_array_ = NULL;
  }
}

double SCW::Predict(const Datum &x) const {
  double predicted_value = 0.0;
  int index_mask = (1 << feature_bit_) - 1;
  for (int i = 0; i < x.num_feature; ++i) {
    predicted_value += mu_array_[x.index[i] & index_mask] * x.value[i];
  }

  return predicted_value;
}

int SCW::UpdateWithPredictedValue(
  const Datum &x, int y, double predicted_value
) {
  
  double s_phi = phi_ * phi_;
  double m = y * predicted_value;

  double v = 0.0;
  int index_mask = (1 << feature_bit_) - 1;
  for (int i = 0; i < x.num_feature; ++i) {
    v += sigma_array_[x.index[i] & index_mask] * x.value[i] * x.value[i];
  }

  double loss = phi_ * std::sqrt(v) - m;
  if (loss <= 0.0) { return 0; } // no need to update

  double alpha = 0.0;
  if (scw_type_ == SCW1) {
    alpha = (-m*psi_ + std::sqrt((m*m*s_phi*s_phi)/4.0 + v*s_phi*xi_)) / (v*xi_);
    if (0.0 > alpha) { alpha = 0.0; }
    if (alpha > C_ ) { alpha = C_;  }

  } else if (scw_type_ == SCW2) {
    double n = v + 0.5 / C_;
    double gamma = phi_* std::sqrt(s_phi*m*m*v*v + 4.0*n*v*(n + v*s_phi));
    alpha = (-(2.0*m*n + s_phi*m*v) + gamma) / (2.0*(n*n + n*v*s_phi));
    if (0.0 > alpha) { alpha = 0.0; }

  }
  if (0.0 >= alpha) { return 0; } // no need to update

  double u = -alpha*v*phi_ + std::sqrt(alpha*alpha*v*v*s_phi + 4.0*v);
  u = (u*u) / 4.0;

  double beta = (alpha*phi_) / (std::sqrt(u) + v*alpha*phi_);

  for (int i = 0; i < x.num_feature; ++i) {
    int j = x.index[i] & index_mask;
    double tmp = sigma_array_[j] * x.value[i];
    mu_array_[j]    += alpha * y * tmp;
    sigma_array_[j] -= beta * tmp * tmp;
  }

  return 1;
}

int SCW::Update(const Datum &x, int y) {
  double predicted_value = Predict(x);
  return UpdateWithPredictedValue(x, y, predicted_value);
}

int SCW::Save(Serializer *serializer) {
  if (serializer == NULL || ! serializer->IsInitialized()) {
    return -1;
  }

  int feature_num = 1 << feature_bit_;
  serializer->SerializeInt(scw_type_);
  serializer->SerializeInt(feature_bit_);
  serializer->SerializeDouble(C_);
  serializer->SerializeDouble(phi_);
  serializer->SerializeDoubleArray(mu_array_, feature_num);
  serializer->SerializeDoubleArray(sigma_array_, feature_num);

  return 1;
}

int SCW::Load(Deserializer *deserializer) {
    if (deserializer == NULL || ! deserializer->IsInitialized()) {
      return -1;
    }

    int ret = 0;
    int type = 0;
    ret = deserializer->DeserializeInt(&type);
    if (ret != 1) { return -1; }
    scw_type_ = SCWType(type);
    ret = deserializer->DeserializeInt(&feature_bit_);
    if (ret != 1) { return -1; }
    ret = deserializer->DeserializeDouble(&C_);
    if (ret != 1) { return -1; }
    ret = deserializer->DeserializeDouble(&phi_);
    if (ret != 1) { return -1; }
    psi_ = 1.0 + (phi_ * phi_) / 2.0;
    xi_  = 1.0 + (phi_ * phi_);

    int feature_num = 1 << feature_bit_;
    if (mu_array_ != NULL) {
      delete[] mu_array_; mu_array_ = NULL;
    }
    mu_array_ = new double[feature_num];
    ret = deserializer->DeserializeDoubleArray(feature_num, mu_array_);
    if (ret != 1) {
      delete mu_array_; mu_array_ = NULL;
      return -1;
    }
    sigma_array_ = new double[feature_num];
    ret = deserializer->DeserializeDoubleArray(feature_num, sigma_array_);
    if (ret != 1) {
      delete sigma_array_; sigma_array_ = NULL;
      return -1;
    }

    return 1;
}


bool SCW::IsInitialized() {
  bool ret = false;
  ret = mu_array_ != NULL && sigma_array_ != NULL &&
        0 <= C_ && phi_ >= 0.0 && 
        (0 <= scw_type_ && scw_type_ < SCW_TYPE_NUM);
  return ret;
}

} // namespace online
} // namespace toybox
