#include "opencv_helpers.hpp"
#include "HOGImageMaskFeature.hpp"
#include <cstdio>
#include "features.hpp"

namespace Picarus {

HOGImageMaskFeature::HOGImageMaskFeature(int bin_size) : bin_size(bin_size) {
}

HOGImageMaskFeature::~HOGImageMaskFeature() {
}

double *HOGImageMaskFeature::compute_mask_feature(unsigned char *image, int height, int width, int *height_out, int *width_out, int *depth_out) {
    double *image_rgb = new double[height * width * 3];
    int image_size = height * width * 3;
    for (int i = 0; i < image_size; i += 3)
        for (int j = 0; j < 3; ++j)
            image_rgb[i + 2 - j] = image[i + j];
    int feature_shape[3];
    process_feat_size(height, width, bin_size, feature_shape);
    int size = feature_shape[0] * feature_shape[1] * feature_shape[2];
    double *mask = new double[size];
    process(image_rgb, height, width, bin_size, mask, size);
    // Transpose from (chan, width, height) -> (height, width, chan)
    // while removing the last dimension from chan as it is always 0 in this
    // feature.  It was used in a later stage in the original code for detection.
    double *mask_out = new double[(feature_shape[0] - 1) * feature_shape[1] * feature_shape[2]];
    for (int i = 0; i < feature_shape[2] - 1; ++i) // depth
        for (int j = 0; j < feature_shape[1]; ++j) // width
            for (int k = 0; k < feature_shape[0]; ++k) // height
                mask_out[(k * feature_shape[1] + j) * (feature_shape[2] - 1) + i] = mask[(i * feature_shape[1] + j) * feature_shape[0] + k];
    *height_out = feature_shape[0];
    *width_out = feature_shape[1];
    *depth_out = feature_shape[2] - 1;
    delete [] image_rgb;
    delete [] mask;
    return mask_out;
}
} // namespace Picarus
