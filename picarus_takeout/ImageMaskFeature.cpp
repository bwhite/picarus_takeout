#include "ImageMaskFeature.hpp"
#include <cstring>

namespace Picarus {
ImageMaskFeature::ImageMaskFeature() {
}
ImageMaskFeature::~ImageMaskFeature() {
}

void ImageMaskFeature::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    int height, width;
    unsigned char * image = image_bgr_fromstring(input, size, &height, &width);
    int height_out;
    int width_out;
    int depth_out;
    double* feature = compute_mask_feature(image, height, width, &height_out, &width_out, &depth_out);
    std::vector<double> vec(feature, feature + height_out * width_out * depth_out);  // TODO: Remove copy!
    std::vector<int> shape(3);
    shape[0] = height_out;
    shape[1] = width_out;
    shape[2] = depth_out;

    ndarray_tostring(vec, shape, collector);
    delete [] feature;
    delete [] image;
}

} // namespace Picarus
