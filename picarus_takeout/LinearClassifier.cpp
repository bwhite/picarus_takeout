#include "LinearClassifier.hpp"
#include <cmath>
#include "picarus_math.h"


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
