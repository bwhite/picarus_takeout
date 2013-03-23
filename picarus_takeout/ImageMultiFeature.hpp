#ifndef IMAGE_MULTI_FEATURE
#define IMAGE_MULTI_FEATURE
#include <vector>
#include "Model.hpp"

namespace Picarus {
class ImageMultiFeature : public Model {
public:
    ImageMultiFeature();
    ~ImageMultiFeature();
    virtual double *compute_multi_feature(unsigned char *image, int height, int width, int *num_features_out, int *feature_size_out) = 0;
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};
} // namespace Picarus
#endif
