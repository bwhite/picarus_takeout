#include "KernelClassifier.hpp"
#include <cmath>
#include <cstring>
#include <cstdio>
#include "picarus_math.h"
#include "kernels_aux.h"

namespace Picarus {
KernelClassifier::KernelClassifier(double *support_vectors, double *dual_coef, int num_support_vectors, int num_coefficients, double intercept, std::string kernel) :  num_support_vectors(num_support_vectors), num_coefficients(num_coefficients), intercept(intercept) {
    this->support_vectors = new double[num_support_vectors * num_coefficients];
    memcpy(this->support_vectors, support_vectors, sizeof(double) * num_support_vectors * num_coefficients);
    this->dual_coef = new double[num_support_vectors];
    memcpy(this->dual_coef, dual_coef, sizeof(double) * num_support_vectors);
    this->kernel_temp = new double[num_support_vectors];
    if (kernel.compare("hik") == 0)
        this->kernel_index = 0;
    else 
        this->kernel_index = -1;
}

KernelClassifier::~KernelClassifier() {
    delete [] support_vectors;
    delete [] kernel_temp;
    delete [] dual_coef;
}

double KernelClassifier::decision_function(double *feature, int size) {
    // Verify size TODO: Is this how we want to exit if there is an error?
    if (size != num_coefficients)
        return NAN;
    // NOTE(brandyn): We don't have to clear kernel_temp because it is set in the kernels themselves
    switch (kernel_index) {
    case 0:
        histogram_intersection_fast(support_vectors, feature, num_support_vectors, 1, num_coefficients, kernel_temp);
        break;
    default:
        return NAN;
    }
    return intercept - dot_product(kernel_temp, dual_coef, num_support_vectors);
}

void KernelClassifier::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    std::vector<double> vec;
    std::vector<int> shape;
    ndarray_fromstring(input, size, &vec, &shape);
    double conf = decision_function(&vec[0], vec.size());
    double_tostring(conf, collector);
}
} // namespace Picarus
