#ifndef TOYBOX_ONLINE_CONFIDENCEWEIGHTED_H
#define TOYBOX_ONLINE_CONFIDENCEWEIGHTED_H

#include <vector>
#include <utility>

namespace toybox {
namespace online {

class Serializer;
class Deserializer;

class ConfidenceWeighted {
  public:
    ConfidenceWeighted();
    ConfidenceWeighted(int feature_bit, double a, double phi);
    ~ConfidenceWeighted();
    double Predict(const std::vector<std::pair<int, double> > &x) const;
    int UpdateWithPredictedValue(
        const std::vector<std::pair<int, double> > &x,
        int y, double predicted_value);
    int Update(const std::vector<std::pair<int, double> > &x, int y);
    int Save(Serializer *serializer);
    int Load(Deserializer *deserializer);
    bool IsInitialized();
    int feature_bit() const { return feature_bit_; }
    double phi() const { return phi_; }

  private:
    int feature_bit_;
    double phi_;
    double psi_;
    double xi_;
    double *mu_array_;
    double *sigma_array_;
    ConfidenceWeighted(const ConfidenceWeighted&);
    void operator=(const ConfidenceWeighted&);
};


} // namespace online
} // namespace toybox

#endif // TOYBOX_ONLINE_CONFIDENCEWEIGHTED_H

