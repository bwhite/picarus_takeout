#ifndef HOG_IMAGE_MASK_FEATURE
#define HOG_IMAGE_MASK_FEATURE
#include <string>
#include <vector>
#include "ImageMaskFeature.hpp"

namespace Picarus {
class HOGImageMaskFeature: public ImageMaskFeature {
private:
    int bin_size;
public:
    HOGImageMaskFeature(int bin_size);
    virtual ~HOGImageMaskFeature();
    virtual double *compute_mask_feature(unsigned char *image, int height, int width, int *height_out, int *width_out, int *depth_out);
};
} // namespace Picarus
#endif
