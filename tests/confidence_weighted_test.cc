#include <cfloat>
#include <vector>

#include <cppunit/extensions/HelperMacros.h>

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
  std::vector<std::pair<int, double> > x;

  const char *filename = "confidence_weighted_save_test1.txt";

  int ret = 0;
  int feature_bit = 5;
  double phi = 0.5;
  ConfidenceWeighted cw(feature_bit, 1.0, phi);

  x.clear();
  x.push_back(std::make_pair(1, 1.0));
  ret = cw.Update(x, +1);

  x.clear();
  x.push_back(std::make_pair(3, 1.0));
  x.push_back(std::make_pair(1, 0.5));
  ret = cw.Update(x, -1);

  x.clear();
  x.push_back(std::make_pair(3,  0.5));
  x.push_back(std::make_pair(1,  2.0));
  x.push_back(std::make_pair(2, -1.0));
  ret = cw.Update(x, -1);

  x.clear();
  x.push_back(std::make_pair(1,  1.0));
  x.push_back(std::make_pair(2,  0.5));
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
  x.clear();
  x.push_back(std::make_pair(1,  1.0));
  x.push_back(std::make_pair(2, -1.0));
  x.push_back(std::make_pair(3,  1.0));
  x.push_back(std::make_pair(4, -1.0));
  x.push_back(std::make_pair(5,  1.0));

  double expected_value  = cw.Predict(x);
  double predicted_value = loaded_cw.Predict(x);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);

  remove(filename);

}

} // namespace online
} // namespace toybox
