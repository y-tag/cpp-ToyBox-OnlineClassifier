#include <cfloat>
#include <vector>

#include <cppunit/extensions/HelperMacros.h>

#include "perceptron.h"
#include "simple_text_serializer.h"
#include "simple_text_deserializer.h"

namespace toybox {
namespace online {

class PerceptronTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(PerceptronTest);
  CPPUNIT_TEST(testUpdateAndPredict);
  CPPUNIT_TEST(testSaveAndLoad);
  CPPUNIT_TEST_SUITE_END();

  public:
    void setUp();
    void tearDown();
    void testUpdateAndPredict();
    void testSaveAndLoad();
};

CPPUNIT_TEST_SUITE_REGISTRATION(PerceptronTest);

void PerceptronTest::setUp() { }

void PerceptronTest::tearDown() { }

void PerceptronTest::testUpdateAndPredict() {
  std::vector<std::pair<int, double> > x;

  // true weights
  // 1:0.5 2:2.0 3:-3.0

  double predicted_value = 0.0;
  double expected_value  = 0.0;
  int ret = 0;
  int feature_bit = 5;
  Perceptron pe(feature_bit);
  CPPUNIT_ASSERT_EQUAL(true, pe.IsInitialized());

  // 0, weights are all zero
  // y:+1 1:1.0
  x.clear();
  x.push_back(std::make_pair(1, 1.0));
  predicted_value = pe.Predict(x);
  expected_value = 0.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);
  ret = pe.UpdateWithPredictedValue(x, +1, predicted_value);
  // 1 ->  0.0 + +1.0 * 1.0 = 1.0
  CPPUNIT_ASSERT_EQUAL(1, ret);

  // y:-1 3:1.0 1:0.5
  x.clear();
  x.push_back(std::make_pair(3, 1.0));
  x.push_back(std::make_pair(1, 0.5));
  predicted_value = pe.Predict(x);
  expected_value = 1.0 * 0.5;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);
  ret = pe.Update(x, -1);
  // 1 ->  1.0 + -1.0 * 0.5 =  0.5
  // 3 ->  0.0 + -1.0 * 1.0 = -1.0
  CPPUNIT_ASSERT_EQUAL(1, ret);

  // y:-1 3:0.5 1:2.0 2:-1.0
  x.clear();
  x.push_back(std::make_pair(3,  0.5));
  x.push_back(std::make_pair(1,  2.0));
  x.push_back(std::make_pair(2, -1.0));
  predicted_value = pe.Predict(x);
  expected_value = 0.5 * 2.0 + -1.0 * 0.5;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);
  ret = pe.UpdateWithPredictedValue(x, -1, predicted_value);
  // 1 ->  0.5 + -1.0 *  2.0 =  1.5
  // 2 ->  0.0 + -1.0 * -1.0 =  1.0
  // 3 -> -1.0 + -1.0 *  0.5 = -1.5
  CPPUNIT_ASSERT_EQUAL(1, ret);

  // 3, weights-> 1:-1.5 2:1.0 3:-1.5
  // y:+1 1:1.0 2:0.5
  x.clear();
  x.push_back(std::make_pair(1, 1.0));
  x.push_back(std::make_pair(2, 0.5));
  predicted_value = pe.Predict(x);
  expected_value = -1.5 * 1.0 + 1.0 * 0.5;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);
  ret = pe.Update(x, +1);
  // 1 -> -1.5 +  1.0 *  1.0 = -0.5
  // 2 ->  1.0 +  1.0 *  0.5 =  1.5
  // 3 -> -1.5
  CPPUNIT_ASSERT_EQUAL(1, ret);

  // check weights
  // weight-> 1:-0.5 2:1.5 3:-1.5
  x.clear();
  x.push_back(std::make_pair(1, 1.0));
  predicted_value = pe.Predict(x);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5, predicted_value, DBL_EPSILON);
  x.clear();
  x.push_back(std::make_pair(2, 1.0));
  predicted_value = pe.Predict(x);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, predicted_value, DBL_EPSILON);
  x.clear();
  x.push_back(std::make_pair(3, 1.0));
  predicted_value = pe.Predict(x);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.5, predicted_value, DBL_EPSILON);

}

void PerceptronTest::testSaveAndLoad() {
  std::vector<std::pair<int, double> > x;

  const char *filename = "perceptron_save_test1.txt";

  int ret = 0;
  int feature_bit = 5;
  Perceptron pe(feature_bit);

  x.clear();
  x.push_back(std::make_pair(1, 1.0));
  ret = pe.Update(x, +1);

  x.clear();
  x.push_back(std::make_pair(3, 1.0));
  x.push_back(std::make_pair(1, 0.5));
  ret = pe.Update(x, -1);

  x.clear();
  x.push_back(std::make_pair(3,  0.5));
  x.push_back(std::make_pair(1,  2.0));
  x.push_back(std::make_pair(2, -1.0));
  ret = pe.Update(x, -1);

  x.clear();
  x.push_back(std::make_pair(1,  1.0));
  x.push_back(std::make_pair(2,  0.5));
  ret = pe.Update(x, +1);

  // save
  Serializer *serializer = new SimpleTextSerializer(filename);
  ret = pe.Save(serializer);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  delete serializer; serializer = NULL;

  // load
  Deserializer *deserializer = new SimpleTextDeserializer(filename);
  Perceptron loaded_pe;
  CPPUNIT_ASSERT_EQUAL(false, loaded_pe.IsInitialized());
  ret = loaded_pe.Load(deserializer);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  CPPUNIT_ASSERT_EQUAL(true, loaded_pe.IsInitialized());
  delete deserializer; deserializer = NULL;

  CPPUNIT_ASSERT_EQUAL(pe.feature_bit(), loaded_pe.feature_bit());

  // for predict
  x.clear();
  x.push_back(std::make_pair(1,  1.0));
  x.push_back(std::make_pair(2, -1.0));
  x.push_back(std::make_pair(3,  1.0));
  x.push_back(std::make_pair(4, -1.0));
  x.push_back(std::make_pair(5,  1.0));

  double expected_value  = pe.Predict(x);
  double predicted_value = loaded_pe.Predict(x);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);

  remove(filename);

}

} // namespace online
} // namespace toybox
