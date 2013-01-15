#ifndef TOYBOX_ONLINE_SERIALIZER_H
#define TOYBOX_ONLINE_SERIALIZER_H

namespace toybox {
namespace online {

class Serializer {
  public:
    Serializer() {};
    virtual ~Serializer() {};
    virtual int SerializeInt(int i) = 0;
    virtual int SerializeDouble(double d) = 0;
    virtual int SerializeDoubleArray(double *array, int length) = 0;
    virtual bool IsInitialized() = 0;
};

} // namespace online
} // namespace toybox

#endif // TOYBOX_ONLINE_SERIALIZER_H

