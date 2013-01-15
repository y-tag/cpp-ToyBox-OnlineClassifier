#include "confidence_weighted.h"

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

ConfidenceWeighted::ConfidenceWeighted()
  : feature_bit_(0), phi_(0.0), psi_(0.0), xi_(0.0),
    mu_array_(NULL), sigma_array_(NULL) {
}

ConfidenceWeighted::ConfidenceWeighted(int feature_bit, double a, double phi)
  : feature_bit_(feature_bit), phi_(phi), psi_(0.0), xi_(0.0),
    mu_array_(NULL), sigma_array_(NULL) {

    psi_ = 1.0 + (phi_ * phi_) / 2.0;
    xi_  = 1.0 + (phi_ * phi_);
    int feature_num = 1 << feature_bit;
    mu_array_    = new double[feature_num];
    sigma_array_ = new double[feature_num];
    std::fill_n(mu_array_,    feature_num, 0.0);
    std::fill_n(sigma_array_, feature_num, a);
}

ConfidenceWeighted::~ConfidenceWeighted() {
  if (mu_array_ != NULL) {
    delete[] mu_array_; mu_array_ = NULL;
  }
  if (sigma_array_ != NULL) {
    delete[] sigma_array_; sigma_array_ = NULL;
  }
}

double ConfidenceWeighted::Predict(const Datum &x) const {
  double predicted_value = 0.0;
  int index_mask = (1 << feature_bit_) - 1;
  for (int i = 0; i < x.num_feature; ++i) {
    predicted_value += mu_array_[x.index[i] & index_mask] * x.value[i];
  }

  return predicted_value;
}

int ConfidenceWeighted::UpdateWithPredictedValue(
  const Datum &x, int y, double predicted_value
) {
  
  double s_phi = phi_ * phi_;
  double m = y * predicted_value;

  double v = 0.0;
  int index_mask = (1 << feature_bit_) - 1;
  for (int i = 0; i < x.num_feature; ++i) {
    v += sigma_array_[x.index[i] & index_mask] * x.value[i] * x.value[i];
  }

  double alpha = (-m*psi_ + std::sqrt((m*m*s_phi*s_phi)/4.0 + v*s_phi*xi_)) / (v*xi_);
  if (0.0 >= alpha) { return 0; } // no need to update

  double u = -alpha*v*phi_ + std::sqrt(alpha*alpha*v*v*s_phi + 4.0*v);
  u = (u*u) / 4.0;

  // not use beta for diag sigma
  //double beta = (alpha*phi_) / (std::sqrt(u) + v*alpha*phi_);

  double mu_coe    = alpha * y;
  double sigma_coe = (alpha * phi_) / std::sqrt(u);
  for (int i = 0; i < x.num_feature; ++i) {
    int j = x.index[i] & index_mask;
    mu_array_[j] += mu_coe * sigma_array_[j] * x.value[i];
    sigma_array_[j]
      = 1.0 / ((1.0 / sigma_array_[j]) + sigma_coe * x.value[i] * x.value[i]);
    //sigma_array_[j] -= beta * sigma_array_[j] * sigma_array_[j] * x.value[i] * x.value[i];
  }

  return 1;
}

int ConfidenceWeighted::Update(const Datum &x, int y) {
  double predicted_value = Predict(x);
  return UpdateWithPredictedValue(x, y, predicted_value);
}

int ConfidenceWeighted::Save(Serializer *serializer) {
  if (serializer == NULL || ! serializer->IsInitialized()) {
    return -1;
  }

  int feature_num = 1 << feature_bit_;
  serializer->SerializeInt(feature_bit_);
  serializer->SerializeDouble(phi_);
  serializer->SerializeDoubleArray(mu_array_, feature_num);
  serializer->SerializeDoubleArray(sigma_array_, feature_num);

  return 1;
}

int ConfidenceWeighted::Load(Deserializer *deserializer) {
    if (deserializer == NULL || ! deserializer->IsInitialized()) {
      return -1;
    }

    int ret = 0;
    ret = deserializer->DeserializeInt(&feature_bit_);
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

bool ConfidenceWeighted::IsInitialized() {
  bool ret = false;
  ret = mu_array_ != NULL && sigma_array_ != NULL && phi_ >= 0.0;
  return ret;
}

} // namespace online
} // namespace toybox
