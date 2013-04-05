#include "ImageFeature2d.hpp"
#include <cstring>

namespace Picarus {

ImageFeature2d::ImageFeature2d() {
}
ImageFeature2d::~ImageFeature2d() {
}


void ImageFeature2d::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    int height, width;
    unsigned char * image = image_bgr_fromstring(input, size, &height, &width);

    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    compute_feature2d(image, height, width, &descriptors, &keypoints);

    std::vector<double> keypoints_vec(keypoints.size() * 5);
    for (int i = 0; i < keypoints.size(); ++i) {
        keypoints_vec[i * 5] = keypoints[i].pt.y;
        keypoints_vec[i * 5 + 1] = keypoints[i].pt.x;
        keypoints_vec[i * 5 + 2] = keypoints[i].angle;
        keypoints_vec[i * 5 + 3] = keypoints[i].response;
        keypoints_vec[i * 5 + 4] = keypoints[i].octave;
    }
    std::vector<int> shape(2);
    shape[0] = descriptors.rows;
    shape[1] = descriptors.cols;
    std::string descriptors_vec(descriptors.data, descriptors.data + descriptors.rows * descriptors.cols);
    binary_feature2d_tostring(descriptors_vec, keypoints_vec, shape, collector);
}
} // namespace Picarus
