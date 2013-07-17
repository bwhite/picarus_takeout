#ifndef FREAK_IMAGE_FEATURE2D
#define FREAK_IMAGE_FEATURE2D
#include "ImageFeature2d.hpp"

namespace Picarus {
class FREAKImageFeature2d: public ImageFeature2d {
private:
    const int thresh;
    cv::FREAK *freak;
public:
    FREAKImageFeature2d(int thresh, bool orientation_norm, bool scale_norm, double pattern_scale, int octaves);
    virtual ~FREAKImageFeature2d();
    virtual void compute_feature2d(unsigned char *image, int height, int width, cv::Mat *descriptors, std::vector<cv::KeyPoint> *keypoints);
};
} // namespace Picarus
#endif
