#include "arow.h"

#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <vector>
#include <utility>

#include "datum.h"
#include "serializer.h"
#include "deserializer.h"

namespace toybox {
namespace online {

AROW::AROW()
  : feature_bit_(0), r_(0.0), mu_array_(NULL), sigma_array_(NULL) {
}

AROW::AROW(int feature_bit, double r)
  : feature_bit_(feature_bit), r_(r),
    mu_array_(NULL), sigma_array_(NULL) {

    int feature_num = 1 << feature_bit;
    mu_array_    = new double[feature_num];
    sigma_array_ = new double[feature_num];
    std::fill_n(mu_array_,    feature_num, 0.0);
    std::fill_n(sigma_array_, feature_num, 1.0);
}


AROW::~AROW() {
  if (mu_array_ != NULL) {
    delete[] mu_array_; mu_array_ = NULL;
  }
  if (sigma_array_ != NULL) {
    delete[] sigma_array_; sigma_array_ = NULL;
  }
}

double AROW::Predict(const Datum &x) const {
  double predicted_value = 0.0;
  int index_mask = (1 << feature_bit_) - 1;
  for (int i = 0; i < x.num_feature; ++i) {
    predicted_value += mu_array_[x.index[i] & index_mask] * x.value[i];
  }

  return predicted_value;
}

int AROW::UpdateWithPredictedValue(
  const Datum &x, int y, double predicted_value
) {
  double loss = 1.0 - y * predicted_value;
  if (loss < 0.0) { return 0; } // no need to update

  double beta = 0.0;
  int index_mask = (1 << feature_bit_) - 1;
  for (int i = 0; i < x.num_feature; ++i) {
    beta += sigma_array_[x.index[i] & index_mask] * x.value[i] * x.value[i];
  }
  beta = 1.0 / (beta + r_);

  double alpha = loss * beta;
  // update mu and sigma
  for (int i = 0; i < x.num_feature; ++i) {
    int j = x.index[i] & index_mask;
    double tmp = sigma_array_[j] * x.value[i];
    mu_array_[j]    += alpha * y * tmp;
    sigma_array_[j] -= beta * tmp * tmp;
  }

  return 1;
}

int AROW::Update(const Datum &x, int y) {
  double predicted_value = Predict(x);
  return UpdateWithPredictedValue(x, y, predicted_value);
}

int AROW::Save(Serializer *serializer) {
  if (serializer == NULL || ! serializer->IsInitialized()) {
    return -1;
  }

  int feature_num = 1 << feature_bit_;
  serializer->SerializeInt(feature_bit_);
  serializer->SerializeDouble(r_);
  serializer->SerializeDoubleArray(mu_array_, feature_num);
  serializer->SerializeDoubleArray(sigma_array_, feature_num);

  return 1;
}

int AROW::Load(Deserializer *deserializer) {
    if (deserializer == NULL || ! deserializer->IsInitialized()) {
      return -1;
    }

    int ret = 0;
    ret = deserializer->DeserializeInt(&feature_bit_);
    if (ret != 1) { return -1; }
    ret = deserializer->DeserializeDouble(&r_);
    if (ret != 1) { return -1; }

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

bool AROW::IsInitialized() {
  bool ret = false;
  ret = mu_array_ != NULL && sigma_array_ != NULL;
  return ret;
}

} // namespace online
} // namespace toybox
