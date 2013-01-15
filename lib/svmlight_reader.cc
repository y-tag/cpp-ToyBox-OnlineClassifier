#include "svmlight_reader.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <fstream>
#include <string>
#include <vector>

#include "datum.h"

namespace {

inline int my_getline(FILE *fp, std::string *buff) {
  if (fp == NULL || buff == NULL) {
    return -1;
  }
  buff->clear();

  int c_buff_len = 1024;
  char c_buff[c_buff_len];

  int ret = 0;
  do {
    if (fgets(c_buff, c_buff_len, fp) == NULL) {
      break;
    }
    ret = 1;
    (*buff) += c_buff;
  } while (buff->rfind('\n') == std::string::npos); 

  while (buff->size() > 0) {
    std::string::size_type nline_pos = buff->rfind('\n');
    if (nline_pos == std::string::npos) {
      break;
    }
    buff->erase(nline_pos, buff->size() - nline_pos);
  }

  return ret;
}
  
} // namespace


namespace toybox {
namespace online {

SVMLightReader::SVMLightReader(const char *in_file) : ifp_(NULL) {
  ifp_ = fopen(in_file, "r");
}

SVMLightReader::~SVMLightReader() {
  if (ifp_ != NULL) {
    fclose(ifp_);
  }
}

int SVMLightReader::Read(Datum *x, int *y) {
  if (x == NULL || y == NULL) {
    return -1;
  }

  std::vector<int>    index_vector;
  std::vector<double> value_vector;

  std::string buff;
  if (my_getline(ifp_, &buff) != 1) {
    return 0;
  }

  char cbuff[buff.size() + 1];
  memmove(cbuff, buff.c_str(), buff.size() + 1);
  char *p = strtok(cbuff, " \t");
  *y = static_cast<int>(strtol(p, NULL, 10));
  while (1) {
    char *f = strtok(NULL, ":");
    char *v = strtok(NULL, " \t");
    if (v == NULL) {
      break;
    }
    index_vector.push_back(static_cast<int>(strtol(f, NULL, 10)));
    value_vector.push_back(strtod(v, NULL));
  }

  if (index_vector.size() > static_cast<size_t>(x->num_reserved)) {
    if (x->num_reserved < 16) { x->num_reserved = 16; }
    do {
      x->num_reserved *= 2;
    } while (index_vector.size() >= static_cast<size_t>(x->num_reserved));
    if (x->index != NULL) { delete[] x->index; }
    if (x->value != NULL) { delete[] x->value; }
    x->index = new int[x->num_reserved];
    x->value = new double[x->num_reserved];
  }

  x->num_feature = index_vector.size();
  for (size_t i = 0; i < index_vector.size(); ++i) {
    x->index[i] = index_vector[i];
    x->value[i] = value_vector[i];
  }

  return 1;
}

int SVMLightReader::Rewind() {
  fseek(ifp_, 0, SEEK_SET);
  return 1;
}

bool SVMLightReader::IsInitialized() {
  return (ifp_ != NULL);
}


} // namespace online
} // namespace toybox
