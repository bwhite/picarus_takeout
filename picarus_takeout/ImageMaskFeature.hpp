#ifndef IMAGE_MASK_FEATURE
#define IMAGE_MASK_FEATURE
#include <vector>
#include "Model.hpp"

namespace Picarus {
class ImageMaskFeature : public Model {
public:
    ImageMaskFeature();
    ~ImageMaskFeature();
    virtual double *compute_mask_feature(unsigned char *image, int height, int width, int *height_out, int *width_out, int *depth_out) = 0;
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};
} // namespace Picarus
#endif
