#include <cfloat>
#include <vector>

#include <cppunit/extensions/HelperMacros.h>

#include "simple_text_serializer.h"
#include "simple_text_deserializer.h"

namespace toybox {
namespace online {

class SimpleTextSerializerTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(SimpleTextSerializerTest);
  CPPUNIT_TEST(testOK);
  CPPUNIT_TEST(testNG);
  CPPUNIT_TEST_SUITE_END();

  public:
    void setUp();
    void tearDown();
    void testOK();
    void testNG();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SimpleTextSerializerTest);

void SimpleTextSerializerTest::setUp() { }

void SimpleTextSerializerTest::tearDown() { }

void SimpleTextSerializerTest::testOK() {
  const char *filename = "simple_text_serializer_test1.txt";

  int i1 =  1234;
  int i2 =  0;
  int i3 = -5678;
  double d1 =  12.34;
  double d2 =  0.0;
  double d3 = -0.5678;

  int length = 8;
  double *array = new double[length];
  array[0] =  1.2;
  array[1] = -3.4;
  array[2] =  0.0;
  array[3] = -5.67;
  array[4] =  0.0;
  array[5] =  0.0;
  array[6] =  0.0;
  array[7] =  8.9;

  int ret = 0;

  {
    SimpleTextSerializer serializer(filename);
    CPPUNIT_ASSERT_EQUAL(true, serializer.IsInitialized());

    ret = serializer.SerializeInt(i1);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    ret = serializer.SerializeDouble(d1);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    ret = serializer.SerializeDouble(d2);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    ret = serializer.SerializeInt(i2);
    CPPUNIT_ASSERT_EQUAL(1, ret);

    ret = serializer.SerializeInt(length);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    ret = serializer.SerializeDoubleArray(array, length);
    CPPUNIT_ASSERT_EQUAL(1, ret);

    ret = serializer.SerializeInt(i3);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    ret = serializer.SerializeDouble(d3);
    CPPUNIT_ASSERT_EQUAL(1, ret);
  }
  
  {
    SimpleTextDeserializer deserializer(filename);
    CPPUNIT_ASSERT_EQUAL(true, deserializer.IsInitialized());
    int    i = 0;
    double d = 0.0;

    ret = deserializer.DeserializeInt(&i);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    CPPUNIT_ASSERT_EQUAL(i1, i);
    ret = deserializer.DeserializeDouble(&d);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(d1, d, DBL_EPSILON);
    ret = deserializer.DeserializeDouble(&d);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(d2, d, DBL_EPSILON);
    ret = deserializer.DeserializeInt(&i);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    CPPUNIT_ASSERT_EQUAL(i2, i);

    ret = deserializer.DeserializeInt(&i);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    CPPUNIT_ASSERT_EQUAL(length, i);

    double *deserialized_array = new double[i];
    ret = deserializer.DeserializeDoubleArray(i, deserialized_array);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    for (int j = 0; j < length; ++j) {
      CPPUNIT_ASSERT_DOUBLES_EQUAL(array[j], deserialized_array[j], DBL_EPSILON);
    }
    delete[] deserialized_array; deserialized_array = NULL;

    ret = deserializer.DeserializeInt(&i);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    CPPUNIT_ASSERT_EQUAL(i3, i);
    ret = deserializer.DeserializeDouble(&d);
    CPPUNIT_ASSERT_EQUAL(1, ret);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(d3, d, DBL_EPSILON);
  }

  delete[] array; array = NULL;

  remove(filename);

}

void SimpleTextSerializerTest::testNG() {
}

} // namespace online
} // namespace toybox
