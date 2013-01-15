#include <cfloat>
#include <vector>

#include <cppunit/extensions/HelperMacros.h>

#include "datum.h"
#include "confidence_weighted.h"
#include "simple_text_serializer.h"
#include "simple_text_deserializer.h"

namespace toybox {
namespace online {

class ConfidenceWeightedTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ConfidenceWeightedTest);
  CPPUNIT_TEST(testSaveAndLoad);
  CPPUNIT_TEST_SUITE_END();

  public:
    void setUp();
    void tearDown();
    void testSaveAndLoad();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ConfidenceWeightedTest);

void ConfidenceWeightedTest::setUp() { }

void ConfidenceWeightedTest::tearDown() { }

void ConfidenceWeightedTest::testSaveAndLoad() {
  Datum x;
  x.num_reserved = 32;
  x.index = new int[x.num_reserved];
  x.value = new double[x.num_reserved];

  const char *filename = "confidence_weighted_save_test1.txt";

  int ret = 0;
  int feature_bit = 5;
  double phi = 0.5;
  ConfidenceWeighted cw(feature_bit, 1.0, phi);

  x.num_feature = 1;
  x.index[0] = 1; x.value[0] =  1.0;
  ret = cw.Update(x, +1);

  x.num_feature = 2;
  x.index[0] = 3; x.value[0] =  1.0;
  x.index[1] = 1; x.value[1] =  0.5;
  ret = cw.Update(x, -1);

  x.num_feature = 3;
  x.index[0] = 3; x.value[0] =  0.5;
  x.index[1] = 1; x.value[1] =  2.0;
  x.index[2] = 2; x.value[2] = -1.0;
  ret = cw.Update(x, -1);

  x.num_feature = 2;
  x.index[0] = 1; x.value[0] =  1.0;
  x.index[1] = 2; x.value[1] =  0.5;
  ret = cw.Update(x, +1);

  // save
  Serializer *serializer = new SimpleTextSerializer(filename);
  ret = cw.Save(serializer);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  delete serializer; serializer = NULL;

  // load
  Deserializer *deserializer = new SimpleTextDeserializer(filename);
  ConfidenceWeighted loaded_cw;
  CPPUNIT_ASSERT_EQUAL(false, loaded_cw.IsInitialized());
  ret = loaded_cw.Load(deserializer);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  CPPUNIT_ASSERT_EQUAL(true, loaded_cw.IsInitialized());
  delete deserializer; deserializer = NULL;

  CPPUNIT_ASSERT_EQUAL(cw.feature_bit(), loaded_cw.feature_bit());
  CPPUNIT_ASSERT_EQUAL(cw.phi(), loaded_cw.phi());

  // for predict
  x.num_feature = 5;
  x.index[0] = 1; x.value[0] =  1.0;
  x.index[1] = 2; x.value[1] = -1.0;
  x.index[2] = 3; x.value[2] =  1.0;
  x.index[3] = 4; x.value[3] = -1.0;
  x.index[4] = 5; x.value[4] =  1.0;

  double expected_value  = cw.Predict(x);
  double predicted_value = loaded_cw.Predict(x);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);

  remove(filename);

}

} // namespace online
} // namespace toybox
