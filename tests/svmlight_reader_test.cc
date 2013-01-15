#include <cfloat>
#include <vector>

#include <cppunit/extensions/HelperMacros.h>

#include "svmlight_reader.h"
#include "datum.h"

namespace toybox {
namespace online {

class SVMLightReaderTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(SVMLightReaderTest);
  CPPUNIT_TEST(testOK);
  CPPUNIT_TEST(testNG);
  CPPUNIT_TEST_SUITE_END();

  public:
    void setUp();
    void tearDown();
    void testOK();
    void testNG();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SVMLightReaderTest);

void SVMLightReaderTest::setUp() { }

void SVMLightReaderTest::tearDown() { }

void SVMLightReaderTest::testOK() {
  Datum x;
  int y = 0;

  int ret = 0;
  const char *filename = "files/svmlight_reader_test1.txt";

  SVMLightReader reader(filename);

  CPPUNIT_ASSERT_EQUAL(true, reader.IsInitialized());

  ret = reader.Read(&x, &y);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  CPPUNIT_ASSERT_EQUAL(1, y);
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(x.num_feature));
  CPPUNIT_ASSERT(x.num_feature <= x.num_reserved);
  CPPUNIT_ASSERT_EQUAL(1,             x.index[0]);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.6,   x.value[0], DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(3,             x.index[1]);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,   x.value[1], DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(4,             x.index[2]);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.1,  x.value[2], DBL_EPSILON);

  ret = reader.Read(&x, &y);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  CPPUNIT_ASSERT_EQUAL(4, y);
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(x.num_feature));
  CPPUNIT_ASSERT(x.num_feature <= x.num_reserved);
  CPPUNIT_ASSERT_EQUAL(2,             x.index[0]);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.4,   x.value[0], DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(5,             x.index[1]);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1,  x.value[1], DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(6,             x.index[2]);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, x.value[2], DBL_EPSILON);

  ret = reader.Read(&x, &y);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  CPPUNIT_ASSERT_EQUAL(2, y);
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(x.num_feature));
  CPPUNIT_ASSERT(x.num_feature <= x.num_reserved);
  CPPUNIT_ASSERT_EQUAL(4,             x.index[0]);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.2,  x.value[0], DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(9,             x.index[1]);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7,   x.value[1], DBL_EPSILON);

  // reach end of file
  ret = reader.Read(&x, &y);
  CPPUNIT_ASSERT_EQUAL(0, ret);

  CPPUNIT_ASSERT_EQUAL(true, reader.IsInitialized());

  // recheck
  ret = reader.Read(&x, &y);
  CPPUNIT_ASSERT_EQUAL(0, ret);

  // rewind
  ret = reader.Rewind();
  CPPUNIT_ASSERT_EQUAL(1, ret);

  // reread
  ret = reader.Read(&x, &y);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  CPPUNIT_ASSERT_EQUAL(1, y);
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(x.num_feature));
  CPPUNIT_ASSERT(x.num_feature <= x.num_reserved);
  CPPUNIT_ASSERT_EQUAL(1,             x.index[0]);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.6,   x.value[0], DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(3,             x.index[1]);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,   x.value[1], DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(4,             x.index[2]);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.1,  x.value[2], DBL_EPSILON);
}

void SVMLightReaderTest::testNG() {
  Datum x;
  int y = 0;

  int ret = 0;

  // try to open invalid file
  SVMLightReader reader1("files/not_exist.txt");
  CPPUNIT_ASSERT_EQUAL(false, reader1.IsInitialized());

  const char *filename = "files/svmlight_reader_test1.txt";
  SVMLightReader reader2(filename);
  CPPUNIT_ASSERT_EQUAL(true, reader2.IsInitialized());

  // x is NULL
  ret = reader2.Read(NULL, &y);
  CPPUNIT_ASSERT_EQUAL(-1, ret);

  // y is NULL
  ret = reader2.Read(&x, NULL);
  CPPUNIT_ASSERT_EQUAL(-1, ret);

  // both are NULL
  ret = reader2.Read(NULL, NULL);
  CPPUNIT_ASSERT_EQUAL(-1, ret);

}

} // namespace online
} // namespace toybox
