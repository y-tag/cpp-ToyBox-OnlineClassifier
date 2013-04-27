#ifndef TOYBOX_ONLINE_SVMLIGHT_READER_H
#define TOYBOX_ONLINE_SVMLIGHT_READER_H

#include <cstdio>
#include <vector>
#include <utility>

namespace toybox {
namespace online {

class SVMLightReader {
  public:
    explicit SVMLightReader(const char *in_file);
    ~SVMLightReader();
    int Read(std::vector<std::pair<int, double> > *x, int *y);
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

