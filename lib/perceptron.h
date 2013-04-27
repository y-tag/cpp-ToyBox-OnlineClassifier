#ifndef TOYBOX_ONLINE_PERCEPTRON_H
#define TOYBOX_ONLINE_PERCEPTRON_H

#include <vector>
#include <utility>

namespace toybox {
namespace online {

class Serializer;
class Deserializer;

class Perceptron {
  public:
    Perceptron();
    explicit Perceptron(int feature_bit);
    ~Perceptron();
    double Predict(const std::vector<std::pair<int, double> > &x) const;
    int UpdateWithPredictedValue(
        const std::vector<std::pair<int, double> > &x,
        int y, double predicted_value);
    int Update(const std::vector<std::pair<int, double> > &x, int y);
    int Save(Serializer *serializer);
    int Load(Deserializer *deserializer);
    bool IsInitialized();
    int feature_bit() const { return feature_bit_; }

  private:
    int feature_bit_;
    double *w_array_;
    Perceptron(const Perceptron&);
    void operator=(const Perceptron&);
};


} // namespace online
} // namespace toybox

#endif // TOYBOX_ONLINE_PERCEPTRON_H

