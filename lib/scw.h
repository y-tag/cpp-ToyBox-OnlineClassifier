#ifndef TOYBOX_ONLINE_SCW_H
#define TOYBOX_ONLINE_SCW_H

#include <vector>
#include <utility>

namespace toybox {
namespace online {

class Serializer;
class Deserializer;

enum SCWType {
  SCW1 = 0, SCW2,
  SCW_TYPE_NUM
};

class SCW {
  public:
    SCW();
    SCW(SCWType type, int feature_bit, double phi, double C);
    ~SCW();
    double Predict(const std::vector<std::pair<int, double> > &x) const;
    int UpdateWithPredictedValue(
        const std::vector<std::pair<int, double> > &x,
        int y, double predicted_value);
    int Update(const std::vector<std::pair<int, double> > &x, int y);
    int Save(Serializer *serializer);
    int Load(Deserializer *deserializer);
    bool IsInitialized();
    SCWType scw_type() const { return scw_type_; }
    int feature_bit() const { return feature_bit_; }
    double C() const { return C_; }
    double phi() const { return phi_; }

  private:
    SCWType scw_type_;
    int feature_bit_;
    double C_;
    double phi_;
    double psi_;
    double xi_;
    double *mu_array_;
    double *sigma_array_;
    SCW(const SCW&);
    void operator=(const SCW&);
};


} // namespace online
} // namespace toybox

#endif // TOYBOX_ONLINE_SCW_H

