#include "LinearClassifier.hpp"
#include <cmath>
#include <cstring>
#include <cstdio>
#include "picarus_math.h"

namespace Picarus {
LinearClassifier::LinearClassifier(double *coefficients, int num_coefficients, double intercept) :  num_coefficients(num_coefficients), intercept(intercept) {
    this->coefficients = new double[num_coefficients];
    memcpy(this->coefficients, coefficients, sizeof(double) * num_coefficients);
}

LinearClassifier::~LinearClassifier() {
    delete [] coefficients;
}

double LinearClassifier::decision_function(double *feature, int size) {
    // Verify size TODO: Is this how we want to exit if there is an error?
    if (size != num_coefficients)
        return NAN;
    return dot_product(coefficients, feature, num_coefficients) + intercept;
}

void LinearClassifier::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    std::vector<double> vec;
    std::vector<int> shape;
    ndarray_fromstring(input, size, &vec, &shape);
    double conf = decision_function(&vec[0], vec.size());
    double_tostring(conf, collector);
}
} // namespace Picarus
