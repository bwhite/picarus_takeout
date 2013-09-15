#ifndef HOMOGRAPHY_RANSAC
#define HOMOGRAPHY_RANSAC
#include <string>
#include <vector>
#include "Model.hpp"

namespace Picarus {
class HomographyRansac : public Model {
private:
    const int min_inliers;
    const double reproj_thresh;
public:
    HomographyRansac(int min_inliers, double reproj_thresh);
    virtual ~HomographyRansac();
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};
} // namespace Picarus
#endif
