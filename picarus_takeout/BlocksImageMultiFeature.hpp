#ifndef BLOCKS_IMAGE_MULTI_FEATURE
#define BLOCKS_IMAGE_MULTI_FEATURE
#include <string>
#include <vector>
#include "ImageMultiFeature.hpp"

namespace Picarus {
class BlocksImageMultiFeature: public ImageMultiFeature {
private:
    int block_size;
    std::string mode;
    int levels;
    int code;
    int skip_cvt_color;
    float min_vals[3];
    float max_vals[3];
public:
    BlocksImageMultiFeature(int block_size, std::string mode, int levels);
    ~BlocksImageMultiFeature();
    virtual double *compute_multi_feature(unsigned char *image, int height, int width, int *num_features_out, int *feature_size_out);
};
} // namespace Picarus
#endif
