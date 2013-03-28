#ifndef FACE_IMAGE_OBJECT_DETECTOR
#define FACE_IMAGE_OBJECT_DETECTOR
#include <vector>
#include "ImageObjectDetector.hpp"
#include "opencv_helpers.hpp"

namespace Picarus {
class FaceImageObjectDetector : public ImageObjectDetector {
private:
    const double scale_factor;
    const int min_neighbors;
    const int min_size;
    const int max_size;
    cv::CascadeClassifier *cascade;
public:
    FaceImageObjectDetector(double scale_factor, int min_neighbors, int min_size, int max_size);
    virtual ~FaceImageObjectDetector();
    virtual double* compute_detections(unsigned char *image, int height, int width, int *out_num_detections);
};
} // namespace Picarus
#endif
