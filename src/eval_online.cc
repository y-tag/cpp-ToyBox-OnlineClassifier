#include "perceptron.h"
#include "passive_aggressive.h"
#include "confidence_weighted.h"
#include "arow.h"
#include "scw.h"
#include "svmlight_reader.h"

#include <boost/math/special_functions/erf.hpp>

#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <map>

int main(int argc, char **argv) {

  if (argc < 3) {
    fprintf(stderr, "Usage: %s train_file test_file\n", argv[0]);
    return 1;
  }

  const char *train_file = argv[1];
  const char *test_file  = argv[2];

  toybox::online::SVMLightReader train_reader(train_file);
  toybox::online::SVMLightReader test_reader(test_file);
  if (! train_reader.IsInitialized()) {
    fprintf(stderr, "Fail to open train_file: %s\n", train_file);
    return 1;
  }
  if (! test_reader.IsInitialized()) {
    fprintf(stderr, "Fail to open test_file: %s\n", test_file);
    return 1;
  }

  int ret = 0;
  std::vector<std::pair<int, double> > x;
  int y = 0;

  struct params {
    double C;
    double phi;
    double r;
  } prm;

  int feature_bit = 20;
  prm.C = 1.0;
  double eta = 0.6;
  prm.phi = sqrt(2.0) * boost::math::erf_inv(2.0 * eta - 1.0);
  prm.r = 0.5;

  //toybox::online::Perceptron classifier(feature_bit);
  //toybox::online::PassiveAggressive classifier(toybox::online::PA,  feature_bit, prm.C);
  //toybox::online::PassiveAggressive classifier(toybox::online::PA1, feature_bit, prm.C);
  //toybox::online::PassiveAggressive classifier(toybox::online::PA2, feature_bit, prm.C);
  //toybox::online::ConfidenceWeighted classifier(feature_bit, 1.0, prm.phi);
  //toybox::online::AROW classifier(feature_bit, prm.r);
  toybox::online::SCW classifier(toybox::online::SCW1, feature_bit, prm.phi, prm.C);
  //toybox::online::SCW classifier(toybox::online::SCW2, feature_bit, prm.phi, prm.C);

  while (train_reader.Read(&x, &y) == 1) {
    classifier.Update(x, y);
  }

  int count_all = 0;
  int count_ok  = 0;

  while (test_reader.Read(&x, &y) == 1) {
    double predicted_value = classifier.Predict(x);
    if (predicted_value * y > 0.0) {
      count_ok += 1;
    }
    //classifier.UpdateWithPredictedValue(x, y, predicted_value);
    count_all += 1;
  }

  fprintf(stderr, "%d / %d = %.4f\n", count_ok, count_all, static_cast<double>(count_ok) / count_all);

  return 0;
}
