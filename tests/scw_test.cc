#include <cfloat>
#include <vector>

#include <cppunit/extensions/HelperMacros.h>

#include "datum.h"
#include "scw.h"
#include "simple_text_serializer.h"
#include "simple_text_deserializer.h"

namespace toybox {
namespace online {

class SCWTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(SCWTest);
  CPPUNIT_TEST(testSaveAndLoad);
  CPPUNIT_TEST_SUITE_END();

  public:
    void setUp();
    void tearDown();
    void testSaveAndLoad();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SCWTest);

void SCWTest::setUp() { }

void SCWTest::tearDown() { }

void SCWTest::testSaveAndLoad() {
  Datum x;
  x.num_reserved = 32;
  x.index = new int[x.num_reserved];
  x.value = new double[x.num_reserved];

  const char *filename = "scw_save_test1.txt";

  int ret = 0;
  int feature_bit = 5;
  double phi = 0.5;
  double C   = 1.0;
  SCW scw(SCW2, feature_bit, phi, C);

  x.num_feature = 1;
  x.index[0] = 1; x.value[0] =  1.0;
  ret = scw.Update(x, +1);

  x.num_feature = 2;
  x.index[0] = 3; x.value[0] =  1.0;
  x.index[1] = 1; x.value[1] =  0.5;
  ret = scw.Update(x, -1);

  x.num_feature = 3;
  x.index[0] = 3; x.value[0] =  0.5;
  x.index[1] = 1; x.value[1] =  2.0;
  x.index[2] = 2; x.value[2] = -1.0;
  ret = scw.Update(x, -1);

  x.num_feature = 2;
  x.index[0] = 1; x.value[0] =  1.0;
  x.index[1] = 2; x.value[1] =  0.5;
  ret = scw.Update(x, +1);

  // save
  Serializer *serializer = new SimpleTextSerializer(filename);
  ret = scw.Save(serializer);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  delete serializer; serializer = NULL;

  // load
  Deserializer *deserializer = new SimpleTextDeserializer(filename);
  SCW loaded_scw;
  CPPUNIT_ASSERT_EQUAL(false, loaded_scw.IsInitialized());
  ret = loaded_scw.Load(deserializer);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  CPPUNIT_ASSERT_EQUAL(true, loaded_scw.IsInitialized());
  delete deserializer; deserializer = NULL;

  CPPUNIT_ASSERT_EQUAL(scw.feature_bit(), loaded_scw.feature_bit());
  CPPUNIT_ASSERT_EQUAL(scw.C(), loaded_scw.C());
  CPPUNIT_ASSERT_EQUAL(scw.phi(), loaded_scw.phi());

  // for predict
  x.num_feature = 5;
  x.index[0] = 1; x.value[0] =  1.0;
  x.index[1] = 2; x.value[1] = -1.0;
  x.index[2] = 3; x.value[2] =  1.0;
  x.index[3] = 4; x.value[3] = -1.0;
  x.index[4] = 5; x.value[4] =  1.0;

  double expected_value  = scw.Predict(x);
  double predicted_value = loaded_scw.Predict(x);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_value, predicted_value, DBL_EPSILON);

  remove(filename);

}

} // namespace online
} // namespace toybox
