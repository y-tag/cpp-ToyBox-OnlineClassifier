#ifndef TOYBOX_ONLINE_SIMPLE_TEXT_DESERIALIZER_H
#define TOYBOX_ONLINE_SIMPLE_TEXT_DESERIALIZER_H

#include <cstdio>

#include "deserializer.h"

namespace toybox {
namespace online {

class SimpleTextDeserializer : public Deserializer {
  public:
    explicit SimpleTextDeserializer(const char *in_file);
    virtual ~SimpleTextDeserializer();
    virtual int DeserializeInt(int *i);
    virtual int DeserializeDouble(double *d);
    virtual int DeserializeDoubleArray(int length, double *array);
    virtual bool IsInitialized();
  private:
    FILE *ifp_;
    SimpleTextDeserializer();
    SimpleTextDeserializer(const SimpleTextDeserializer&);
    void operator=(const SimpleTextDeserializer&);
};

} // namespace online
} // namespace toybox

#endif // TOYBOX_ONLINE_SIMPLE_TEXT_SERIALIZER_H

