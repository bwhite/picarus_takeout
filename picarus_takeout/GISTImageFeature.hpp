#ifndef GIST_IMAGE_FEATURE
#define GIST_IMAGE_FEATURE
#include <string>
#include <vector>
#include "ImageFeature.hpp"

namespace Picarus {
class GISTImageFeature: public ImageFeature {
private:
    const int num_blocks;
    const std::vector<int> orientations_per_scale;
    int descriptor_size;
public:
    GISTImageFeature(int num_blocks, std::vector<int> orientations_per_scale);
    ~GISTImageFeature();
    double *compute_feature(unsigned char *image, int height, int width, int *out_size);
};
} // namespace Picarus
#endif
