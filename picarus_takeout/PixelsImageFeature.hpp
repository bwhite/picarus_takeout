#ifndef PIXELS_IMAGE_FEATURE
#define PIXELS_IMAGE_FEATURE
#include <string>
#include <vector>
#include "ImageFeature.hpp"

namespace Picarus {
class PixelsImageFeature: public ImageFeature {
private:
    std::string mode;
    int code;
    int skip_cvt_color;
    float min_vals[3];
    float max_vals[3];
public:
    PixelsImageFeature(std::string mode);
    ~PixelsImageFeature();
    double *compute_feature(unsigned char *image, int height, int width, int *out_size);
};
} // namespace Picarus
#endif
