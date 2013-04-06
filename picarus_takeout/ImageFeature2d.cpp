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

    std::vector<double> keypoints_vec(keypoints.size() * 6);
    for (int i = 0; i < keypoints.size(); ++i) {
        keypoints_vec[i * 6] = keypoints[i].pt.y / height;
        keypoints_vec[i * 6 + 1] = keypoints[i].pt.x / width;
        keypoints_vec[i * 6 + 2] = keypoints[i].angle;
        keypoints_vec[i * 6 + 3] = keypoints[i].response;
        keypoints_vec[i * 6 + 4] = keypoints[i].octave;
        keypoints_vec[i * 6 + 5] = keypoints[i].size;
    }
    std::vector<int> shape(2);
    shape[0] = descriptors.rows;
    shape[1] = descriptors.cols;
    std::string descriptors_vec(descriptors.data, descriptors.data + descriptors.rows * descriptors.cols);
    binary_feature2d_tostring(descriptors_vec, keypoints_vec, shape, collector);
    delete [] image;
}
} // namespace Picarus
