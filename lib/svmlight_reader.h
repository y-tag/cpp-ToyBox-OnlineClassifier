#ifndef TOYBOX_ONLINE_SVMLIGHT_READER_H
#define TOYBOX_ONLINE_SVMLIGHT_READER_H

#include <cstdio>

namespace toybox {
namespace online {

struct Datum;

class SVMLightReader {
  public:
    explicit SVMLightReader(const char *in_file);
    ~SVMLightReader();
    int Read(Datum *x, int *y); // this might delete index and value in x
    int Rewind();
    bool IsInitialized();

  private:
    FILE *ifp_;
    SVMLightReader();
    SVMLightReader(const SVMLightReader&);
    void operator=(const SVMLightReader&);
};


} // namespace online
} // namespace toybox

#endif // TOYBOX_ONLINE_SVMLIGHT_READER_H

