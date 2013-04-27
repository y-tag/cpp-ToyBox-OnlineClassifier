#include "passive_aggressive.h"

#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <vector>
#include <utility>

#include "serializer.h"
#include "deserializer.h"

namespace toybox {
namespace online {

PassiveAggressive::PassiveAggressive()
  : pa_type_(PA), feature_bit_(0), C_(0.0), w_array_(NULL) {
}

PassiveAggressive::PassiveAggressive(PAType type, int feature_bit, double C)
  : pa_type_(type), feature_bit_(feature_bit), C_(C), w_array_(NULL) {
    int feature_num = 1 << feature_bit;
    w_array_ = new double[feature_num];
    std::fill_n(w_array_, feature_num, 0.0);
}

PassiveAggressive::~PassiveAggressive() {
  if (w_array_ != NULL) {
    delete[] w_array_; w_array_ = NULL;
  }
}

double PassiveAggressive::Predict(
    const std::vector<std::pair<int, double> > &x) const {
  double predicted_value = 0.0;
  int index_mask = (1 << feature_bit_) - 1;
  for (size_t i = 0; i < x.size(); ++i) {
    predicted_value += w_array_[x[i].first & index_mask] * x[i].second;
  }

  return predicted_value;
}

int PassiveAggressive::UpdateWithPredictedValue(
    const std::vector<std::pair<int, double> > &x, int y, double predicted_value
) {
  
  double loss = 1.0 - y * predicted_value;

  if (loss > 0.0) {
    double squared_x_norm = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
      squared_x_norm += x[i].second * x[i].second;
    }

    double eta = 0.0;
    if (pa_type_ == PA) {
      eta = loss / squared_x_norm;
    } else if (pa_type_ == PA1) {
      eta = loss / squared_x_norm;
      if (C_ < eta) { eta = C_; }
    } else if (pa_type_ == PA2) {
      eta = loss / (squared_x_norm + (0.5 / C_));
    }

    int index_mask = (1 << feature_bit_) - 1;
    for (size_t i = 0; i < x.size(); ++i) {
      w_array_[x[i].first & index_mask] += eta * y * x[i].second;
    }
  }

  return 1;
}

int PassiveAggressive::Update(
    const std::vector<std::pair<int, double> > &x, int y) {

  double predicted_value = 0.0;
  double squared_x_norm  = 0.0;
  int index_mask = (1 << feature_bit_) - 1;
  for (size_t i = 0; i < x.size(); ++i) {
    predicted_value += w_array_[x[i].first & index_mask] * x[i].second;
    squared_x_norm += x[i].second * x[i].second;
  }

  double loss = 1.0 - y * predicted_value;

  if (loss > 0.0) {
    double eta = 0.0;
    if (pa_type_ == PA) {
      eta = loss / squared_x_norm;
    } else if (pa_type_ == PA1) {
      eta = loss / squared_x_norm;
      if (C_ < eta) { eta = C_; }
    } else if (pa_type_ == PA2) {
      eta = loss / (squared_x_norm + (0.5 / C_));
    }

    for (size_t i = 0; i < x.size(); ++i) {
      w_array_[x[i].first & index_mask] += eta * y * x[i].second;
    }
  }

  return 1;
}

int PassiveAggressive::Save(Serializer *serializer) {
  if (serializer == NULL || ! serializer->IsInitialized()) {
    return -1;
  }

  int feature_num = 1 << feature_bit_;
  serializer->SerializeInt(pa_type_);
  serializer->SerializeInt(feature_bit_);
  serializer->SerializeDouble(C_);
  serializer->SerializeDoubleArray(w_array_, feature_num);

  return 1;
}

int PassiveAggressive::Load(Deserializer *deserializer) {
    if (deserializer == NULL || ! deserializer->IsInitialized()) {
      return -1;
    }
    
    int ret = 0;
    int type = 0;
    ret = deserializer->DeserializeInt(&type);
    if (ret != 1) { return -1; }
    pa_type_ = PAType(type); // What is better way?
    ret = deserializer->DeserializeInt(&feature_bit_);
    if (ret != 1) { return -1; }
    ret = deserializer->DeserializeDouble(&C_);
    if (ret != 1) { return -1; }

    int feature_num = 1 << feature_bit_;
    if (w_array_ != NULL) {
      delete[] w_array_; w_array_ = NULL;
    }
    w_array_ = new double[feature_num];
    ret = deserializer->DeserializeDoubleArray(feature_num, w_array_);
    if (ret != 1) {
      delete w_array_; w_array_ = NULL;
      return -1;
    }

    return 1;
}


bool PassiveAggressive::IsInitialized() {
  bool ret = false;
  ret = w_array_ != NULL && 0 <= C_ && 
        (0 <= pa_type_ && pa_type_ < PA_TYPE_NUM);
  return ret;
}


} // namespace online
} // namespace toybox
