#ifndef TOYBOX_ONLINE_AROW_H
#define TOYBOX_ONLINE_AROW_H

#include <vector>
#include <utility>

namespace toybox {
namespace online {

class Serializer;
class Deserializer;

class AROW {
  public:
    AROW();
    AROW(int feature_bit, double r);
    ~AROW();
    double Predict(const std::vector<std::pair<int, double> > &x) const;
    int UpdateWithPredictedValue(
        const std::vector<std::pair<int, double> > &x,
        int y, double predicted_value);
    int Update(const std::vector<std::pair<int, double> > &x, int y);
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

