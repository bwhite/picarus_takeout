#ifndef IMAGE_OBJECT_DETECTOR
#define IMAGE_OBJECT_DETECTOR
#include <vector>
#include "Model.hpp"

namespace Picarus {
class ImageObjectDetector : public Model {
public:
    ImageObjectDetector();
    virtual ~ImageObjectDetector();
    virtual double* compute_detections(unsigned char *image, int height, int width, int *out_num_detections) = 0;
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};
} // namespace Picarus
#endif
