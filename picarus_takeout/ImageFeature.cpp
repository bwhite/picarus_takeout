#include "ImageFeature.hpp"
#include <cstring>

namespace Picarus {

ImageFeature::ImageFeature() {
}
ImageFeature::~ImageFeature() {
}


void ImageFeature::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    int height, width;
    unsigned char * image = image_bgr_fromstring(input, size, &height, &width);
    int feature_size;
    double* feature = compute_feature(image, height, width, &feature_size);
    std::vector<double> vec(feature, feature + feature_size);  // TODO: Remove copy!
    std::vector<int> shape(1);
    shape[0] = feature_size;
    ndarray_tostring(vec, shape, collector);
    delete [] feature;
    delete [] image;
}
} // namespace Picarus
