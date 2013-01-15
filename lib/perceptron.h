#ifndef TOYBOX_ONLINE_PERCEPTRON_H
#define TOYBOX_ONLINE_PERCEPTRON_H

namespace toybox {
namespace online {

struct Datum;
class Serializer;
class Deserializer;

class Perceptron {
  public:
    Perceptron();
    explicit Perceptron(int feature_bit);
    ~Perceptron();
    double Predict(const Datum &x) const;
    int UpdateWithPredictedValue(const Datum &x, int y, double predicted_value);
    int Update(const Datum &x, int y);
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

