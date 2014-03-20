#include "ARMarkerDetector.hpp"
#include "aruco/marker.h"
namespace Picarus {

ARMarkerDetector::ARMarkerDetector() {
    detector = new aruco::MarkerDetector();
}

ARMarkerDetector::~ARMarkerDetector() {
    delete detector;
}

void ARMarkerDetector::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    int height, width;
    unsigned char * image = image_bgr_fromstring(input, size, &height, &width);
    int num_detections;
    double* detections = find_tags(image, height, width, &num_detections);
    std::vector<double> vec(detections, detections + num_detections * 9);  // TODO: Remove copy!
    std::vector<int> shape(2);
    shape[0] = num_detections;
    shape[1] = 9;
    ndarray_tostring(vec, shape, collector);
    
    delete [] detections;
    delete [] image;
}

double* ARMarkerDetector::find_tags(unsigned char *image, int height, int width, int *out_num_detections) {
    cv::Mat image_mat(height, width, CV_8UC3, image);
    std::vector<aruco::Marker> detectedMarkers;
    detector->detect(image_mat, detectedMarkers);
    double *output = new double[9 * detectedMarkers.size()];
    for (int i = 0; i < detectedMarkers.size(); i++) {
      aruco::Marker m = detectedMarkers[i];
      output[i * 9] = m.id;
      for (int j = 0; j < 4; j++) {
        output[i * 9 + (j * 2 + 1)] = m[j].x;
        output[i * 9 + (j * 2 + 2)] = m[j].y;
      }
    }
    *out_num_detections = detectedMarkers.size();
    return output;
}
} // namespace Picarus
