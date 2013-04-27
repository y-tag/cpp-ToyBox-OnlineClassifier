#include <cfloat>
#include <vector>

#include <cppunit/extensions/HelperMacros.h>

#include "svmlight_reader.h"

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
  std::vector<std::pair<int, double> > x;
  int y = 0;

  int ret = 0;
  const char *filename = "files/svmlight_reader_test1.txt";

  SVMLightReader reader(filename);

  CPPUNIT_ASSERT_EQUAL(true, reader.IsInitialized());

  ret = reader.Read(&x, &y);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  CPPUNIT_ASSERT_EQUAL(1, y);
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(x.size()));
  CPPUNIT_ASSERT_EQUAL(1,             x[0].first);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.6,   x[0].second, DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(3,             x[1].first);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,   x[1].second, DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(4,             x[2].first);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.1,  x[2].second, DBL_EPSILON);

  ret = reader.Read(&x, &y);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  CPPUNIT_ASSERT_EQUAL(4, y);
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(x.size()));
  CPPUNIT_ASSERT_EQUAL(2,             x[0].first);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.4,   x[0].second, DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(5,             x[1].first);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1,  x[1].second, DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(6,             x[2].first);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, x[2].second, DBL_EPSILON);

  ret = reader.Read(&x, &y);
  CPPUNIT_ASSERT_EQUAL(1, ret);
  CPPUNIT_ASSERT_EQUAL(2, y);
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(x.size()));
  CPPUNIT_ASSERT_EQUAL(4,             x[0].first);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.2,  x[0].second, DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(9,             x[1].first);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7,   x[1].second, DBL_EPSILON);

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
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(x.size()));
  CPPUNIT_ASSERT_EQUAL(1,             x[0].first);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.6,   x[0].second, DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(3,             x[1].first);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,   x[1].second, DBL_EPSILON);
  CPPUNIT_ASSERT_EQUAL(4,             x[2].first);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.1,  x[2].second, DBL_EPSILON);
}

void SVMLightReaderTest::testNG() {
  std::vector<std::pair<int, double> > x;
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
