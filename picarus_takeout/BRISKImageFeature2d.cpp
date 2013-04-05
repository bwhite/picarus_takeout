#include "BRISKImageFeature2d.hpp"
namespace Picarus {

BRISKImageFeature2d::BRISKImageFeature2d(int thresh, int octaves, double pattern_scale) {
    brisk = new cv::BRISK(thresh, octaves, pattern_scale);
}

BRISKImageFeature2d::~BRISKImageFeature2d() {
    delete brisk;
}

void BRISKImageFeature2d::compute_feature2d(unsigned char *image, int height, int width, cv::Mat *descriptors, std::vector<cv::KeyPoint> *keypoints) {
    cv::Mat image_mat(height, width, CV_8UC3, image);
    cv::Mat mask(image_mat.size(), CV_8UC1, 255);
    (*brisk)(image_mat, mask, *keypoints, *descriptors);
}
} // namespace Picarus
