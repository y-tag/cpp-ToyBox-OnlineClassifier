#include "perceptron.h"

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

Perceptron::Perceptron()
  : feature_bit_(0), w_array_(NULL) {
}

Perceptron::Perceptron(int feature_bit)
  : feature_bit_(feature_bit), w_array_(NULL) {
    int feature_num = 1 << feature_bit;
    w_array_ = new double[feature_num];
    std::fill_n(w_array_, feature_num, 0.0);
}

Perceptron::~Perceptron() {
  if (w_array_ != NULL) {
    delete[] w_array_; w_array_ = NULL;
  }
}

double Perceptron::Predict(const Datum &x) const {
  double predicted_value = 0.0;
  int index_mask = (1 << feature_bit_) - 1;
  for (int i = 0; i < x.num_feature; ++i) {
    predicted_value += w_array_[x.index[i] & index_mask] * x.value[i];
  }

  return predicted_value;
}

int Perceptron::UpdateWithPredictedValue(
  const Datum &x, int y, double predicted_value
) {

  if (y * predicted_value <= 0.0) {
    int index_mask = (1 << feature_bit_) - 1;
    for (int i = 0; i < x.num_feature; ++i) {
      w_array_[x.index[i] & index_mask] += y * x.value[i];
    }
  }

  return 1;
}

int Perceptron::Update(const Datum &x, int y) {
  double predicted_value = Predict(x);
  return UpdateWithPredictedValue(x, y, predicted_value);
}

int Perceptron::Save(Serializer *serializer) {
  if (serializer == NULL || ! serializer->IsInitialized()) {
    return -1;
  }

  int feature_num = 1 << feature_bit_;
  serializer->SerializeInt(feature_bit_);
  serializer->SerializeDoubleArray(w_array_, feature_num);

  return 1;
}

int Perceptron::Load(Deserializer *deserializer) {
    if (deserializer == NULL || ! deserializer->IsInitialized()) {
      return -1;
    }
    
    int ret = 0;
    ret = deserializer->DeserializeInt(&feature_bit_);
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

bool Perceptron::IsInitialized() {
  return (w_array_ != NULL);
}


} // namespace online
} // namespace toybox
