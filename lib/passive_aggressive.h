#ifndef TOYBOX_ONLINE_PASSIVEAGGRESSIVE_H
#define TOYBOX_ONLINE_PASSIVEAGGRESSIVE_H

#include <vector>
#include <utility>

namespace toybox {
namespace online {

class Serializer;
class Deserializer;

enum PAType {
  PA = 0, PA1, PA2,
  PA_TYPE_NUM
};

class PassiveAggressive {
  public:
    PassiveAggressive();
    PassiveAggressive(PAType type, int feature_bit, double C);
    ~PassiveAggressive();
    double Predict(const std::vector<std::pair<int, double> > &x) const;
    int UpdateWithPredictedValue(
        const std::vector<std::pair<int, double> > &x,
        int y, double predicted_value);
    int Update(const std::vector<std::pair<int, double> > &x, int y);
    int Save(Serializer *serializer);
    int Load(Deserializer *deserializer);
    bool IsInitialized();
    PAType pa_type() const { return pa_type_; }
    int feature_bit() const { return feature_bit_; }
    double C() const { return C_; }

  private:
    PAType pa_type_;
    int feature_bit_;
    double C_;
    double *w_array_;
    PassiveAggressive(const PassiveAggressive&);
    void operator=(const PassiveAggressive&);
};


} // namespace online
} // namespace toybox

#endif // TOYBOX_ONLINE_PASSIVEAGGRESSIVE_H

