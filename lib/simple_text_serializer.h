#ifndef TOYBOX_ONLINE_SIMPLE_TEXT_SERIALIZER_H
#define TOYBOX_ONLINE_SIMPLE_TEXT_SERIALIZER_H

#include <cstdio>

#include "serializer.h"

namespace toybox {
namespace online {

class SimpleTextSerializer : public Serializer {
  public:
    explicit SimpleTextSerializer(const char *out_file);
    virtual ~SimpleTextSerializer();
    virtual int SerializeInt(int i);
    virtual int SerializeDouble(double d);
    virtual int SerializeDoubleArray(double *array, int length);
    virtual bool IsInitialized();
  private:
    FILE *ofp_;
    SimpleTextSerializer();
    SimpleTextSerializer(const SimpleTextSerializer&);
    void operator=(const SimpleTextSerializer&);
};

} // namespace online
} // namespace toybox

#endif // TOYBOX_ONLINE_SIMPLE_TEXT_SERIALIZER_H

