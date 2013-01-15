#ifndef TOYBOX_ONLINE_DESERIALIZER_H
#define TOYBOX_ONLINE_DESERIALIZER_H

namespace toybox {
namespace online {

class Deserializer {
  public:
    Deserializer() {};
    virtual ~Deserializer() {};
    virtual int DeserializeInt(int *i) = 0;
    virtual int DeserializeDouble(double *d) = 0;
    virtual int DeserializeDoubleArray(int length, double *array) = 0;
    virtual bool IsInitialized() = 0;
};

} // namespace online
} // namespace toybox

#endif // TOYBOX_ONLINE_DESERIALIZER_H

