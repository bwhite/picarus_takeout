#include "ImageMatcherHammingRansac.hpp"
#include "opencv_helpers.hpp"
#include <cstring>
#include "hamming_aux.h"
#include <iostream>
namespace Picarus {

ImageMatcherHammingRansac::ImageMatcherHammingRansac(int max_dist, int min_inliers, double reproj_thresh) : max_dist(max_dist), min_inliers(min_inliers), reproj_thresh(reproj_thresh) {
}


ImageMatcherHammingRansac::~ImageMatcherHammingRansac() {
}

void ImageMatcherHammingRansac::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    std::string features0, features1;
    std::vector<double> vec0, vec1;
    std::vector<int> shape0, shape1;
    std::pair<std::string, std::string> val;
    string_pair_fromstring(input, size, &val);
    binary_feature2d_fromstring((const unsigned char*) val.first.c_str(), val.first.size(), &features0, &vec0, &shape0);
    binary_feature2d_fromstring((const unsigned char*) val.second.c_str(), val.second.size(), &features1, &vec1, &shape1);
    int num_pts0 = shape0[0];
    int num_pts1 = shape1[0];

    int *dists = new int[num_pts0 * num_pts1];
    // TODO: Check that shape0[1] == shape1[1]
    memset(dists, 0, sizeof(int) * num_pts0 * num_pts1);
    hamdist_cmap_lut16((const unsigned char *)features0.c_str(),
                       (const unsigned char *)features1.c_str(),
                       dists, shape0[1], num_pts0, num_pts1);
    std::vector<cv::Point2f> match0;
    std::vector<cv::Point2f> match1;

    
    for (int i = 0; i < num_pts0; ++i)
        for (int j = 0; j < num_pts1; ++j) {
            if (dists[num_pts1 * i + j] <= max_dist) {
                cv::Point2f pt0(vec0[i * 6 + 1], vec0[i * 6]), pt1(vec1[j * 6 + 1], vec1[j * 6]);
                match0.push_back(pt0);
                match1.push_back(pt1);
            }
        }
    bool matched = false;
    if (match0.size() > 4) {
        cv::Mat mask(match0.size(), 1, CV_8U);
        cv::Mat H = findHomography(match0, match1, CV_RANSAC, reproj_thresh, mask);
        int num_inliers = 0;
        for (int i = 0; i < match0.size(); ++i)
            num_inliers += ((unsigned char *)mask.data)[i] > 0;
        std::cout << H << std::endl;
        std::cout << num_inliers << std::endl;
        matched = num_inliers >= min_inliers;
        std::cout << "Ransac ran" << std::endl;
    }
    delete [] dists;
    if (matched)
        std::cout << "C:Matched" << std::endl;
    else
        std::cout << "C:UnMatched" << std::endl;
    bool_tostring(matched, collector);
}
}
