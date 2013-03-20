#ifndef LINEAR_CLASSIFIER
#define LINEAR_CLASSIFIER
#include <vector>
#include "Model.hpp"

namespace Picarus {
class LinearClassifier : public Model {
private:
    std::vector<double> coefficients;
    double intercept;
public:
    LinearClassifier(std::vector<double> coefficients, double intercept);
    ~LinearClassifier();
    double decision_function(double *feature, int size);
    virtual void process_binary(const unsigned char *input, int size, void (*collector)(const unsigned char *, int, void *), void *collector_state);
};

} // namespace Picarus
#endif
