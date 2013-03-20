#include "LinearClassifier.hpp"
#include <cmath>
#include "picarus_math.h"

namespace Picarus {
LinearClassifier::LinearClassifier(std::vector<double> coefficients, double intercept) :  coefficients(coefficients), intercept(intercept) {
}

LinearClassifier::~LinearClassifier() {
}

double LinearClassifier::decision_function(double *feature, int size) {
    // Verify size TODO: Is this how we want to exit if there is an error?
    if (size != coefficients.size())
        return NAN;
    return dot_product(&coefficients[0], feature, size) + intercept;
}

void LinearClassifier::process_binary(const unsigned char *input, int size, void (*collector)(const unsigned char *, int, void *), void *collector_state) {
    std::vector<double> vec;
    std::vector<int> shape;
    ndarray_fromstring(input, size, &vec, &shape);
    double conf = decision_function(&vec[0], vec.size());
    double_tostring(conf, collector, collector_state);
}
} // namespace Picarus
