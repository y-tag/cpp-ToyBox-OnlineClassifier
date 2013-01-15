#ifndef TOYBOX_ONLINE_DATUM_H
#define TOYBOX_ONLINE_DATUM_H

#include <cstdlib>

namespace toybox {
namespace online {

struct Datum {
  int num_feature;
  int num_reserved;
  int *index;
  double *value;

  Datum() : num_feature(0), num_reserved(0), index(NULL), value(NULL) {}
  ~Datum() {
    if (index != NULL) { delete[] index; index = NULL; }
    if (value != NULL) { delete[] value; index = NULL; }
  }
};

} // namespace online
} // namespace toybox

#endif // TOYBOX_ONLINE_DATUM_H

