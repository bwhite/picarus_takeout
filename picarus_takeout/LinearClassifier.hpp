#ifndef LINEAR_CLASSIFIER
#define LINEAR_CLASSIFIER
#include <vector>
#include "Model.hpp"

namespace Picarus {
class LinearClassifier : public Model {
private:
    double *coefficients;
    const int num_coefficients;
    const double intercept;
public:
    LinearClassifier(double *coefficients, int num_coefficients, double intercept);
    virtual ~LinearClassifier();
    double decision_function(double *feature, int size);
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};

} // namespace Picarus
#endif
