#include "HomographyRansac.hpp"
#include "opencv_helpers.hpp"
#include <cstring>
#include <iostream>
namespace Picarus {

HomographyRansac::HomographyRansac(int min_inliers, double reproj_thresh) : min_inliers(min_inliers), reproj_thresh(reproj_thresh) {
}


HomographyRansac::~HomographyRansac() {
}

void HomographyRansac::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    std::vector<double> points;
    std::vector<int> shape;
    ndarray_fromstring((const unsigned char*) input, size, &points, &shape);
    std::vector<cv::Point2f> match0;
    std::vector<cv::Point2f> match1;
    
    for (int i = 0; i < shape[0]; ++i) {
        cv::Point2f pt0(points[4 * i + 1], points[4 * i]), pt1(points[4 * i + 3], points[4 * i + 2]);
        match0.push_back(pt0);
        match1.push_back(pt1);
    }
    if (match0.size() > 4) {
        cv::Mat mask(match0.size(), 1, CV_8U);
        cv::Mat H = findHomography(match0, match1, CV_RANSAC, reproj_thresh, mask);
        int num_inliers = 0;
        for (int i = 0; i < match0.size(); ++i)
            num_inliers += ((unsigned char *)mask.data)[i] > 0;
        std::cout << H << std::endl;
        if (num_inliers >= min_inliers) {
            std::vector<double> vec((double *)H.data, (double *)(H.data) + 9);
            std::vector<int> shape(2);
            shape[0] = 3;
            shape[1] = 3;
            ndarray_tostring(vec, shape, collector);
        } else {
            (*collector)(NULL, 0);
        }
        std::cout << "Ransac ran - Inliers " << num_inliers << std::endl;
    } else {
        (*collector)(NULL, 0);
    }
}
}
