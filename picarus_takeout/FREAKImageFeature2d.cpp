#include "FREAKImageFeature2d.hpp"
namespace Picarus {

FREAKImageFeature2d::FREAKImageFeature2d(int thresh, bool orientation_norm, bool scale_norm, double pattern_scale, int octaves) : thresh(thresh) {
    freak = new cv::FREAK(orientation_norm, scale_norm, pattern_scale, octaves);
}

FREAKImageFeature2d::~FREAKImageFeature2d() {
    delete freak;
}

void FREAKImageFeature2d::compute_feature2d(unsigned char *image, int height, int width, cv::Mat *descriptors, std::vector<cv::KeyPoint> *keypoints) {
    cv::Mat image_mat(height, width, CV_8UC3, image);
    cv::FAST(image_mat, *keypoints, thresh);
    freak->compute(image_mat, *keypoints, *descriptors);
}
} // namespace Picarus
