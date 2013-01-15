#ifndef TOYBOX_ONLINE_AROW_H
#define TOYBOX_ONLINE_AROW_H

namespace toybox {
namespace online {

struct Datum;
class Serializer;
class Deserializer;

class AROW {
  public:
    AROW();
    AROW(int feature_bit, double r);
    ~AROW();
    double Predict(const Datum &x) const;
    int UpdateWithPredictedValue(const Datum &x, int y, double predicted_value);
    int Update(const Datum &x, int y);
    int Save(Serializer *serializer);
    int Load(Deserializer *deserializer);
    bool IsInitialized();
    int feature_bit() const { return feature_bit_; }
    double r() const { return r_; }

  private:
    int feature_bit_;
    double r_;
    double *mu_array_;
    double *sigma_array_;
    AROW(const AROW&);
    void operator=(const AROW&);
};


} // namespace online
} // namespace toybox

#endif // TOYBOX_ONLINE_AROW_H

