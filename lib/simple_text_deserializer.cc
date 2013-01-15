#include "simple_text_deserializer.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

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

SimpleTextDeserializer::SimpleTextDeserializer(const char *in_file) : ifp_(NULL) {
  ifp_ = fopen(in_file, "r");
}

SimpleTextDeserializer::~SimpleTextDeserializer() {
  if (ifp_ != NULL) {
    fclose(ifp_);
  }
}

int SimpleTextDeserializer::DeserializeInt(int *i) {
  if (i == NULL) { return -1; }

  std::string buff;
  if (my_getline(ifp_, &buff) != 1) {
    return -1;
  }

  if (sscanf(buff.c_str(), "%d", i) == 0) {
    return -1;
  } 

  return 1;
}

int SimpleTextDeserializer::DeserializeDouble(double *d) {
  if (d == NULL) { return -1; }

  std::string buff;
  if (my_getline(ifp_, &buff) != 1) {
    return -1;
  }

  if (sscanf(buff.c_str(), "%lf", d) == 0) {
    return -1;
  } 

  return 1;
}

int SimpleTextDeserializer::DeserializeDoubleArray(int length, double *array) {
  if (length < 1 || array == NULL) {
    return -1;
  }

  std::fill_n(array, length, 0.0);

  std::string buff;
  while (my_getline(ifp_, &buff) == 1) {
    if (buff.find("begin") != std::string::npos) { continue; }
    if (buff.find("end") != std::string::npos)   { break; }

    int    i = 0;
    double v = 0.0;
    if (sscanf(buff.c_str(), "%d%lf", &i, &v) == 2) {
      if (i < length) {
        array[i] = v;
      }
    } 
  }

  return 1;
}

bool SimpleTextDeserializer::IsInitialized() {
  return (ifp_ != NULL);
}

} // namespace online
} // namespace toybox
