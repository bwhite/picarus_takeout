#ifndef KERNEL_CLASSIFIER
#define KERNEL_CLASSIFIER
#include <vector>
#include "Model.hpp"

namespace Picarus {
class KernelClassifier : public Model {
private:
    double *support_vectors;
    double *dual_coeff;
    double *kernel_temp;
    const int num_support_vectors;
    const int num_coefficients;
    const double intercept;
    int kernel_index;
public:
    KernelClassifier(double *support_vectors, double *dual_coeff, int num_support_vectors, int num_coefficients, double intercept, std::string kernel);
    virtual ~KernelClassifier();
    double decision_function(double *feature, int size);
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};

} // namespace Picarus
#endif
