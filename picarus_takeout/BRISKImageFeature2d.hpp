#ifndef BRISK_IMAGE_FEATURE2D
#define BRISK_IMAGE_FEATURE2D
#include "ImageFeature2d.hpp"

namespace Picarus {
class BRISKImageFeature2d: public ImageFeature2d {
private:
    cv::BRISK *brisk;
public:
    BRISKImageFeature2d(int thresh, int octaves, double pattern_scale);
    virtual ~BRISKImageFeature2d();
    virtual void compute_feature2d(unsigned char *image, int height, int width, cv::Mat *descriptors, std::vector<cv::KeyPoint> *keypoints);
};
} // namespace Picarus
#endif
