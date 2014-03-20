#ifndef AR_MARKER_DETECTOR
#define AR_MARKER_DETECTOR
#include "Model.hpp"
#include "aruco/markerdetector.h"

namespace Picarus {
class ARMarkerDetector : public Model {
private:
    aruco::MarkerDetector *detector;
public:
    ARMarkerDetector();
    virtual ~ARMarkerDetector();
    void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
    double* find_tags(unsigned char *image, int height, int width, int *out_num_detections);
};
} // namespace Picarus
#endif
