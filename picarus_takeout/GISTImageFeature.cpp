#include "GISTImageFeature.hpp"
#include "gist/gist_wrapper.h"

namespace Picarus {

GISTImageFeature::GISTImageFeature(int num_blocks, std::vector<int> orientations_per_scale) : num_blocks(num_blocks), orientations_per_scale(orientations_per_scale) {
    descriptor_size = 0;
    for (int i = 0; i < orientations_per_scale.size(); ++i)
        descriptor_size += num_blocks * num_blocks * orientations_per_scale[i];
    descriptor_size *= 3;
}

GISTImageFeature::~GISTImageFeature() {
}

double *GISTImageFeature::compute_feature(unsigned char *image, int height, int width, int *out_size) {
    float *feature_f = new float[descriptor_size];
    double *feature_d = new double[descriptor_size];
    color_gist_scaletab_wrap(image, height, width, num_blocks, orientations_per_scale.size(), &orientations_per_scale[0], feature_f, descriptor_size);
    for (int i = 0; i < descriptor_size; ++i)
        feature_d[i] = feature_f[i];
    *out_size = descriptor_size;
    delete [] feature_f;
    return feature_d;
}
} // namespace Picarus
