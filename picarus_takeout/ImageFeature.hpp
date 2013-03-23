#ifndef IMAGE_FEATURE
#define IMAGE_FEATURE
#include <vector>
#include "Model.hpp"

namespace Picarus {
class ImageFeature : public Model {
public:
    ImageFeature();
    ~ImageFeature();
    virtual double* compute_feature(unsigned char *image, int height, int width, int *out_size) = 0;
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
protected:
    double *pyramid_histogram(unsigned int *label_image, int height, int width, int max_val, int levels, int *out_size);
};
} // namespace Picarus
#endif
