#include "ImageObjectDetector.hpp"
#include <cstring>

namespace Picarus {

ImageObjectDetector::ImageObjectDetector() {
}
ImageObjectDetector::~ImageObjectDetector() {
}


void ImageObjectDetector::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    int height, width;
    unsigned char * image = image_bgr_fromstring(input, size, &height, &width);
    int feature_size;
    int num_detections;
    double* detections = compute_detections(image, height, width, &num_detections);
    std::vector<double> vec(detections, detections + num_detections * 4);  // TODO: Remove copy!
    std::vector<int> shape(2);
    std::string image_str(input, input + size);
    shape[0] = num_detections;
    shape[1] = 4;
    image_detections_tostring(image_str, vec, shape, collector);
    
    delete [] detections;
    delete [] image;
}
} // namespace Picarus
