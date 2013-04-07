#include "HammingFeature2dHashIndex.hpp"
#include <vector>
#include <cstring>
#include "knearest_neighbor.h"
#include "hamming_aux.h"
#include <cstdio>
#include <stdexcept>
#include <algorithm>
#include <cmath>

namespace Picarus {
HammingFeature2dHashIndex::HammingFeature2dHashIndex(const unsigned char *hashes, int *indeces, int num_hashes, int num_bytes,
                                                     const std::vector<std::string> &labels, int max_results, int max_keypoint_results, int hamming_thresh) : num_hashes(num_hashes), num_bytes(num_bytes), labels(labels), max_results(max_results), max_keypoint_results(max_keypoint_results), hamming_thresh(hamming_thresh) {
    printf("num_hashes[%d] num_bytes[%d] max_results[%d] num_labels[%d] max_keypoint_results[%d] hamming_thresh[%d]\n", num_hashes, num_bytes, max_results, labels.size(), max_keypoint_results, hamming_thresh);
    // TODO: Add back in linear hamming distance code
    this->index = new HammingMultiIndex(hashes, num_hashes, num_bytes, max_keypoint_results);
    this->hashes = new unsigned char[num_hashes * num_bytes];
    memcpy(this->hashes, hashes, num_hashes * num_bytes);
    this->indeces = new int[num_hashes];
    this->result_indeces = new int[max_keypoint_results];
    this->result_dists = new double[max_keypoint_results];
    this->temp_hamming = new int[num_hashes];
    memcpy(this->indeces, indeces, num_hashes * sizeof(int));
    // Compute label weights based on # of hashes that belong to each
    idf = new double[labels.size()];
    memset(idf, 0, sizeof(double) * labels.size());
    for (int i = 0; i < num_hashes; ++i)
        idf[indeces[i]] += 1.;
    for (int i = 0; i < labels.size(); ++i)
        idf[i] = 1. / log10(idf[i]);
}

HammingFeature2dHashIndex::~HammingFeature2dHashIndex() {
    delete index;

    delete [] hashes;
    delete [] indeces;
    delete [] idf;
    delete [] result_indeces;
    delete [] result_dists;
    delete [] temp_hamming;
}

std::vector<std::pair<double, std::string> > *HammingFeature2dHashIndex::query_index(const unsigned char *hashes, double *keypoints, int num_hashes, int num_bytes) {
    int i, j, max_valid_ind;
    double prev_dist_val;
    if (this->num_bytes != num_bytes) {
        printf("%d %d\n", this->num_bytes, num_bytes);
        return NULL;
    }
    std::map<int, double> dists;
    /*
    for (i = 0; i < num_hashes; ++i) {
        max_valid_ind = -1;
        place_dist_clear_arrays(result_indeces, result_dists, max_keypoint_results);
        memset(temp_hamming, 0, sizeof(int) * this->num_hashes);
        hamdist_cmap_lut16(this->hashes, hashes + num_bytes * i, temp_hamming, num_bytes, this->num_hashes, 1);
        for (j = 0; j < this->num_hashes; ++j)
            max_valid_ind = place_dist_in_results(j, temp_hamming[j], result_indeces, result_dists, max_keypoint_results, max_valid_ind);
        printf("max_valid_ind[%d]\n", max_valid_ind);
        for (j = 0; j < max_valid_ind + 1; ++j) {
            if (j == 0)
                printf("b: %d %d %f\n", i, j, result_dists[j]);
            if (result_dists[j] > hamming_thresh)
                break;
            try {
                prev_dist_val = dists.at(indeces[result_indeces[j]]);
            } catch (const std::out_of_range& oor) {
                prev_dist_val = 0.;
            }
            dists[indeces[result_indeces[j]]] = prev_dist_val - idf[indeces[result_indeces[j]]];  // TODO(brandyn): Incorporate TF-IDF if num_bytes is small enough to store densely
        }
    }*/
    for (i = 0; i < num_hashes; ++i) {
        int num_results = 0;
        int *results = index->query_index(hashes + num_bytes * i, num_bytes, &num_results);
        for (j = 0; j < num_results; ++j) {
            if (j == 0)
                printf("b: %d %d %d %d\n", i, j, results[j * 2], results[j * 2 + 1]);
            if (results[j * 2] > hamming_thresh)
                break;
            try {
                prev_dist_val = dists.at(indeces[results[j * 2 + 1]]);
            } catch (const std::out_of_range& oor) {
                prev_dist_val = 0.;
            }
            dists[indeces[results[j * 2 + 1]]] = prev_dist_val - idf[indeces[results[j * 2 + 1]]];  // TODO(brandyn): Incorporate TF-IDF if num_bytes is small enough to store densely
        }
        delete [] results;
    }
    std::vector<std::pair<double, std::string> > *dists_out  = new std::vector<std::pair<double, std::string> >();
    const int num_results = dists.size() > max_results ? max_results : dists.size();
    dists_out->reserve(dists.size());
    std::map<int, double>::const_iterator itr;
    for (itr = dists.begin(); itr != dists.end(); ++itr)
        dists_out->push_back(std::make_pair(itr->second, labels[itr->first]));
    std::sort(dists_out->begin(), dists_out->end());
    dists_out->resize(num_results);
    return dists_out;
}

void HammingFeature2dHashIndex::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    std::string features;
    std::vector<double> vec;
    std::vector<int> shape;
    binary_feature2d_fromstring(input, size, &features, &vec, &shape);
    std::vector<std::pair<double, std::string> > *out = query_index((const unsigned char *)features.c_str(), &vec[0], shape[0], shape[1]);
    if (out == NULL)
        (*collector)(NULL, 0);
    else {
        double_strings_tostring(*out, collector);
        delete out;
    }
}

} // namespace Picarus
