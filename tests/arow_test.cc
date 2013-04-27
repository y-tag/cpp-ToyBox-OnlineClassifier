#include <cfloat>
#include <vector>

#include <cppunit/extensions/HelperMacros.h>

#include "arow.h"
#include "simple_text_serializer.h"
#include "simple_text_deserializer.h"

namespace toybox {
namespace online {

class AROWTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(AROWTest);
  CPPUNIT_TEST(testSaveAndLoad);
  CPPUNIT_TEST_SUITE_END();

  public:
    void setUp();
    void tearDown();
    void testSaveAndLoad();
};

CPPUNIT_TEST_SUITE_REGISTRATION(AROWTest);

void AROWTest::setUp() { }

void AROWTest::tearDown() { }

void AROWTest::testSaveAndLoad() {
  std::vector<std::pair<int, double> > x;

  const char *filename = "arow_save_test1.txt";

  int ret = 0;
  int feature_bit = 5;
  double r = 0.5;
  AROW arow(feature_bit, r);

  x.clear();
  x.push_back(std::make_pair(1, 1.0));
  ret = arow.Update(x, +1);

  x.clear();
  x.push_back(std::make_pair(3, 1.0));
  x.push_back(std::make_pair(1, 0.5));
  ret = arow.Update(x, -1);

  x.clear();
  x.push_back(std::make_pair(3,  0.5));
  x.push_back(std::make_pair(1,  2.0));
  x.push_back(std::make_pair(2, -1.0));
  ret = arow.Update(x, -1);

  x.clear();
  x.push_back(std::make_pair(1,  1.0));
  x.push_back(std::make_pair(2,  0.5));
  ret = arow.Update(x, +1);

  // save
  Serializer *serializer = new SimpleTextSerializer(filename);
  ret = arow.Save(serializer);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  delete serializer; serializer = NULL;

  // load
  Deserializer *deserializer = new SimpleTextDeserializer(filename);
  AROW loaded_arow;
  CPPUNIT_ASSERT_EQUAL(false, loaded_arow.IsInitialized());
  ret = loaded_arow.Load(deserializer);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  CPPUNIT_ASSERT_EQUAL(true, loaded_arow.IsInitialized());
  delete deserializer; deserializer = NULL;

  CPPUNIT_ASSERT_EQUAL(arow.feature_bit(), loaded_arow.feature_bit());
  CPPUNIT_ASSERT_EQUAL(arow.r(), loaded_arow.r());

  // for predict
  x.clear();
  x.push_back(std::make_pair(1,  1.0));
  x.push_back(std::make_pair(2, -1.0));
  x.push_back(std::make_pair(3,  1.0));
  x.push_back(std::make_pair(4, -1.0));
  x.push_back(std::make_pair(5,  1.0));

  double expected_value  = arow.Predict(x);
  double predicted_value = loaded_arow.Predict(x);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);

  remove(filename);

}

} // namespace online
} // namespace toybox
