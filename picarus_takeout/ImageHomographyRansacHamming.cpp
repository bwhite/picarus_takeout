#include "ImageHomographyRansacHamming.hpp"
#include "opencv_helpers.hpp"
#include <cstring>
#include "hamming_aux.h"
#include <iostream>
namespace Picarus {

static void min_dist_singleton(int *dists, int num_dists, int *min_dist_out, int *min_ind_out) {
    int min_ind = 0;
    int min_dist = 1000000;  // TODO: Max int
    int cardinality = -1; // Ensures exactly 1 point has this distance
    for (int i = 0; i < num_dists; ++i)
        if (dists[i] == min_dist) {
            ++cardinality;
        } else if (dists[i] < min_dist) {
            cardinality = 1;
            min_ind = i;
            min_dist = dists[i];
        }
    if (cardinality == 1) {
        *min_dist_out = min_dist;
        *min_ind_out = min_ind;
    } else {
        *min_dist_out = 1000000; // TODO: Max int
        *min_ind_out = -2;
    }
}

ImageHomographyRansacHamming::ImageHomographyRansacHamming(int max_dist, int min_inliers, double reproj_thresh) : max_dist(max_dist), min_inliers(min_inliers), reproj_thresh(reproj_thresh) {
}


ImageHomographyRansacHamming::~ImageHomographyRansacHamming() {
}

void ImageHomographyRansacHamming::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    std::string features0, features1;
    std::vector<double> vec0, vec1;
    std::vector<int> shape0, shape1;
    std::pair<std::string, std::string> val;
    string_pair_fromstring(input, size, &val);
    binary_feature2d_fromstring((const unsigned char*) val.first.c_str(), val.first.size(), &features0, &vec0, &shape0);
    binary_feature2d_fromstring((const unsigned char*) val.second.c_str(), val.second.size(), &features1, &vec1, &shape1);
    int num_pts0 = shape0[0];
    int num_pts1 = shape1[0];
    int smalleris0 = num_pts0 <= num_pts1;
    unsigned char *smaller, *bigger;
    int num_pts_smaller, num_pts_bigger;
    if (smalleris0) {
        smaller = (unsigned char *)features0.c_str();
        bigger = (unsigned char *)features1.c_str();
        num_pts_smaller = num_pts0;
        num_pts_bigger = num_pts1;
    } else {
        smaller = (unsigned char *)features1.c_str();
        bigger = (unsigned char *)features0.c_str();
        num_pts_smaller = num_pts1;
        num_pts_bigger = num_pts0;
    }
    // TODO: Check that shape0[1] == shape1[1]
    int *dists_smaller = new int[num_pts_smaller];
    int *dists_bigger = new int[num_pts_bigger];
    int *cache_bigger = new int[num_pts_bigger];
    for (int i = 0; i < num_pts_bigger; ++i)
        cache_bigger[i] = -1;
    std::vector<cv::Point2f> match0;
    std::vector<cv::Point2f> match1;
    for (int i = 0; i < num_pts_smaller; ++i) {
        hamdist_cmap_lut16((const unsigned char *)smaller + shape0[1] * i,
                           (const unsigned char *)bigger,
                           dists_bigger, shape0[1], 1, num_pts_bigger);
        int min_dist0, min_ind0;
        min_dist_singleton(dists_bigger, num_pts_bigger, &min_dist0, &min_ind0);
        if (max_dist < min_dist0 || min_ind0 < 0 || (cache_bigger[min_ind0] != -1 && cache_bigger[min_ind0] != i))
            continue;
        hamdist_cmap_lut16((const unsigned char *)bigger + shape0[1] * min_ind0,
                           (const unsigned char *)smaller,
                           dists_smaller, shape0[1], 1, num_pts_smaller);
        int min_dist1, min_ind1;
        min_dist_singleton(dists_smaller, num_pts_smaller, &min_dist1, &min_ind1);
        cache_bigger[min_ind0] = min_ind1;
        if (cache_bigger[min_ind0] != i)
            continue;
        if (smalleris0) {
            cv::Point2f pt0(vec0[i * 6 + 1], vec0[i * 6]), pt1(vec1[min_ind0 * 6 + 1], vec1[min_ind0 * 6]);
            match0.push_back(pt0);
            match1.push_back(pt1);
        } else {
            cv::Point2f pt0(vec0[min_ind0 * 6 + 1], vec0[min_ind0 * 6]), pt1(vec1[i * 6 + 1], vec1[i * 6]);
            match0.push_back(pt0);
            match1.push_back(pt1);
        }
    }

    std::cout << "Matched " << match0.size() << std::endl;
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
    delete [] dists_smaller;
    delete [] dists_bigger;
    delete [] cache_bigger;
}
}
