#ifndef IMAGE_MATCHER_HAMMING_RANSAC
#define IMAGE_MATCHER_HAMMING_RANSAC
#include <string>
#include <vector>
#include "Model.hpp"

namespace Picarus {
class ImageMatcherHammingRansac : public Model {
private:
    const int max_dist;
    const int min_inliers;
    const double reproj_thresh;
public:
    ImageMatcherHammingRansac(int max_dist, int min_inliers, double reproj_thresh);
    virtual ~ImageMatcherHammingRansac();
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};
} // namespace Picarus
#endif
