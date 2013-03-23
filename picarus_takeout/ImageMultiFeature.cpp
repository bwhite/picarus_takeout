#include "ImageMultiFeature.hpp"
#include <cstring>

namespace Picarus {
ImageMultiFeature::ImageMultiFeature() {
}
ImageMultiFeature::~ImageMultiFeature() {
}

void ImageMultiFeature::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    int height, width;
    unsigned char * image = image_bgr_fromstring(input, size, &height, &width);
    int feature_size;
    int num_features;
    double* feature = compute_multi_feature(image, height, width, &num_features, &feature_size);
    std::vector<double> vec(feature, feature + feature_size * num_features);  // TODO: Remove copy!
    std::vector<int> shape(2);
    shape[0] = num_features;
    shape[1] = feature_size;
    ndarray_tostring(vec, shape, collector);
    delete [] feature;
    delete [] image;
}

} // namespace Picarus
