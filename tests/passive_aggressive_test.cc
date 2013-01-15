#include <cfloat>
#include <vector>

#include <cppunit/extensions/HelperMacros.h>

#include "datum.h"
#include "passive_aggressive.h"
#include "simple_text_serializer.h"
#include "simple_text_deserializer.h"

namespace toybox {
namespace online {

class PassiveAggressiveTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(PassiveAggressiveTest);
  CPPUNIT_TEST(testUpdateAndPredict);
  CPPUNIT_TEST(testSaveAndLoad);
  CPPUNIT_TEST_SUITE_END();

  public:
    void setUp();
    void tearDown();
    void testUpdateAndPredict();
    void testSaveAndLoad();
};

CPPUNIT_TEST_SUITE_REGISTRATION(PassiveAggressiveTest);

void PassiveAggressiveTest::setUp() { }

void PassiveAggressiveTest::tearDown() { }

void PassiveAggressiveTest::testUpdateAndPredict() {
  Datum x;
  x.num_reserved = 32;
  x.index = new int[x.num_reserved];
  x.value = new double[x.num_reserved];

  // true weights
  // 1:0.5 2:2.0 3:-3.0

  double predicted_value = 0.0;
  double expected_value  = 0.0;
  int ret = 0;
  int feature_bit = 5;
  double C = 0.0;
  PassiveAggressive pa(PA, feature_bit, C);

  // y:+1 1:1.0
  x.num_feature = 1;
  x.index[0] = 1; x.value[0] =  1.0;
  predicted_value = pa.Predict(x);
  expected_value = 0.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);
  ret = pa.UpdateWithPredictedValue(x, +1, predicted_value);
  // loss = 1.0 - (+1.0*0.0) = 1.0
  // squared_x_norm = 1.0**2 = 1.0
  // eta = 1.0 / 1.0 = 1.0
  // 1 ->  0.0 + 1.0 * +1.0 * 1.0 = 1.0
  CPPUNIT_ASSERT_EQUAL(1, ret);

  // y:-1 3:1.0 1:0.5
  x.num_feature = 2;
  x.index[0] = 3; x.value[0] =  1.0;
  x.index[1] = 1; x.value[1] =  0.5;
  predicted_value = pa.Predict(x);
  expected_value = 1.0 * 0.5;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);
  ret = pa.Update(x, -1);
  // loss = 1.0 - (-1.0*0.5) = 1.5
  // squared_x_norm = 1.0**2 + 0.5**2 = 1.25
  // eta = 1.5 / 1.25 = 1.2
  // 1 ->  1.0 + 1.2 * -1.0 * 0.5 =  0.4
  // 3 ->  0.0 + 1.2 * -1.0 * 1.0 = -1.2
  CPPUNIT_ASSERT_EQUAL(1, ret);

  // y:-1 3:0.5 1:2.0 2:-1.0
  x.num_feature = 3;
  x.index[0] = 3; x.value[0] =  0.5;
  x.index[1] = 1; x.value[1] =  2.0;
  x.index[2] = 2; x.value[2] = -1.0;
  predicted_value = pa.Predict(x);
  expected_value = 0.4 * 2.0 + -1.2 * 0.5;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);
  ret = pa.UpdateWithPredictedValue(x, -1, predicted_value);
  // loss = 1.0 - (-1.0*0.2) = 1.2
  // squared_x_norm = 0.5**2 + 2.0**2 + -1.0**2 = 5.25
  // eta = 1.2 / 5.25
  // 1 ->  0.4 + (1.2 / 5.25) * -1.0 *  2.0
  // 2 ->  0.0 + (1.2 / 5.25) * -1.0 * -1.0
  // 3 -> -1.2 + (1.2 / 5.25) * -1.0 *  0.5
  CPPUNIT_ASSERT_EQUAL(1, ret);

  // y:+1 1:1.0 2:0.5
  x.num_feature = 2;
  x.index[0] = 1; x.value[0] =  1.0;
  x.index[1] = 2; x.value[1] =  0.5;
  predicted_value = pa.Predict(x);
  expected_value = ( 0.4 + (1.2 / 5.25) * -1.0 *  2.0) * 1.0 +
                   ( 0.0 + (1.2 / 5.25) * -1.0 * -1.0) * 0.5;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);
  ret = pa.Update(x, +1);
  // loss = 1.0 - (-1.0*0.2) = 1.2
  // squared_x_norm = 0.5**2 + 2.0**2 + -1.0**2 = 5.25
  // eta = 1.2 / 5.25
  // 1 ->  1.0 + (1.2 / 5.25) * -1.0 *  2.0
  // 2 ->  0.0 + (1.2 / 5.25) * -1.0 * -1.0
  // 3 ->  0.5 + (1.2 / 5.25) * -1.0 *  0.5
  CPPUNIT_ASSERT_EQUAL(1, ret);

}

void PassiveAggressiveTest::testSaveAndLoad() {
  Datum x;
  x.num_reserved = 32;
  x.index = new int[x.num_reserved];
  x.value = new double[x.num_reserved];

  const char *filename = "passive_aggressive_save_test1.txt";

  int ret = 0;
  int feature_bit = 5;
  double C = 0.1;
  PassiveAggressive pa(PA2, feature_bit, C);

  x.num_feature = 1;
  x.index[0] = 1; x.value[0] =  1.0;
  ret = pa.Update(x, +1);

  x.num_feature = 2;
  x.index[0] = 3; x.value[0] =  1.0;
  x.index[1] = 1; x.value[1] =  0.5;
  ret = pa.Update(x, -1);

  x.num_feature = 3;
  x.index[0] = 3; x.value[0] =  0.5;
  x.index[1] = 1; x.value[1] =  2.0;
  x.index[2] = 2; x.value[2] = -1.0;
  ret = pa.Update(x, -1);

  x.num_feature = 2;
  x.index[0] = 1; x.value[0] =  1.0;
  x.index[1] = 2; x.value[1] =  0.5;
  ret = pa.Update(x, +1);

  // save
  Serializer *serializer = new SimpleTextSerializer(filename);
  ret = pa.Save(serializer);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  delete serializer; serializer = NULL;

  // load
  Deserializer *deserializer = new SimpleTextDeserializer(filename);
  PassiveAggressive loaded_pa;
  CPPUNIT_ASSERT_EQUAL(false, loaded_pa.IsInitialized());
  ret = loaded_pa.Load(deserializer);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  CPPUNIT_ASSERT_EQUAL(true, loaded_pa.IsInitialized());
  delete deserializer; deserializer = NULL;

  CPPUNIT_ASSERT_EQUAL(pa.pa_type(), loaded_pa.pa_type());
  CPPUNIT_ASSERT_EQUAL(pa.feature_bit(), loaded_pa.feature_bit());
  CPPUNIT_ASSERT_EQUAL(pa.C(), loaded_pa.C());

  // for predict
  x.num_feature = 5;
  x.index[0] = 1; x.value[0] =  1.0;
  x.index[1] = 2; x.value[1] = -1.0;
  x.index[2] = 3; x.value[2] =  1.0;
  x.index[3] = 4; x.value[3] = -1.0;
  x.index[4] = 5; x.value[4] =  1.0;

  double expected_value  = pa.Predict(x);
  double predicted_value = loaded_pa.Predict(x);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);

  remove(filename);

}

} // namespace online
} // namespace toybox
