#ifndef IMAGE_FEATURE2D
#define IMAGE_FEATURE2D
#include "opencv_helpers.hpp"
#include <vector>
#include "Model.hpp"

namespace Picarus {
class ImageFeature2d : public Model {
public:
    ImageFeature2d();
    virtual ~ImageFeature2d();
    virtual void compute_feature2d(unsigned char *image, int height, int width, cv::Mat *descriptors, std::vector<cv::KeyPoint> *keypoints) = 0;
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};
} // namespace Picarus
#endif
