#ifndef IMAGE_HOMOGRAPHY_RANSAC_HAMMING
#define IMAGE_HOMOGRAPHY_RANSAC_HAMMING
#include <string>
#include <vector>
#include "Model.hpp"

namespace Picarus {
class ImageHomographyRansacHamming : public Model {
private:
    const int max_dist;
    const int min_inliers;
    const double reproj_thresh;
public:
    ImageHomographyRansacHamming(int max_dist, int min_inliers, double reproj_thresh);
    virtual ~ImageHomographyRansacHamming();
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};
} // namespace Picarus
#endif
