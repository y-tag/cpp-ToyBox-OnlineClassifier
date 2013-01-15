#include "simple_text_serializer.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

namespace toybox {
namespace online {

SimpleTextSerializer::SimpleTextSerializer(const char *out_file) : ofp_(NULL) {
  ofp_ = fopen(out_file, "w");
}

SimpleTextSerializer::~SimpleTextSerializer() {
  if (ofp_ != NULL) {
    fclose(ofp_);
  }
}

int SimpleTextSerializer::SerializeInt(int i) {
  fprintf(ofp_, "%d\n", i);
  return 1;
}

int SimpleTextSerializer::SerializeDouble(double d) {
  fprintf(ofp_, "%.16lf\n", d);
  return 1;
}

int SimpleTextSerializer::SerializeDoubleArray(double *array, int length) {
  if (array == NULL || length < 1) {
    return -1;
  }

  fprintf(ofp_, "begin double array\n");
  for (int i = 0; i < length; ++i) {
    if (array[i] != 0.0) {
      fprintf(ofp_, "%d\t%.16lf\n", i, array[i]);
    }
  }
  fprintf(ofp_, "end double array\n");

  return 1;
}


bool SimpleTextSerializer::IsInitialized() {
  return (ofp_ != NULL);
}

} // namespace online
} // namespace toybox
