#include "FaceImageObjectDetector.hpp"
#include <cstring>

namespace Picarus {
FaceImageObjectDetector::FaceImageObjectDetector(double scale_factor, int min_neighbors, int min_size, int max_size) : scale_factor(scale_factor), min_neighbors(min_neighbors), min_size(min_size), max_size(max_size) {
    cascade = new cv::CascadeClassifier("/usr/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml");
}
FaceImageObjectDetector::~FaceImageObjectDetector() {
    delete cascade;
}

double* FaceImageObjectDetector::compute_detections(unsigned char *image, int height, int width, int *out_num_detections) {
    cv::Mat image_mat(height, width, CV_8UC3, image);
    unsigned char *image_gray_data = new unsigned char[height * width];
    std::vector<cv::Rect> faces;
    cv::Mat image_mat_gray(height, width, CV_8UC1, image_gray_data);
    cv::cvtColor(image_mat, image_mat_gray, CV_BGR2GRAY);
    cv::equalizeHist(image_mat_gray, image_mat_gray);
    cascade->detectMultiScale(image_mat_gray, faces, scale_factor, min_neighbors, 0, cv::Size(min_size, min_size), cv::Size(max_size, max_size));
    double *detections_out = new double[faces.size() * 4];
    for (int i = 0; i < faces.size(); ++i) {
        detections_out[i * 4] = faces[i].y;
        detections_out[i * 4 + 1] = faces[i].y + faces[i].height;
        detections_out[i * 4 + 2] = faces[i].x;
        detections_out[i * 4 + 3] = faces[i].x + faces[i].width;
    }
    delete [] image_gray_data;
    *out_num_detections = faces.size();
    return detections_out;
}

} // namespace Picarus
