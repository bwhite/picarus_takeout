#include "HammingHashIndex.hpp"
#include <vector>
#include <cstring>
#include "knearest_neighbor.h"
#include "hamming_aux.h"
#include <cstdio>

namespace Picarus {
HammingHashIndex::HammingHashIndex(const unsigned char *hashes, int *indeces, int num_hashes, int num_bytes, const std::vector<std::string> &labels, int max_results) : num_hashes(num_hashes), num_bytes(num_bytes), labels(labels), max_results(max_results) {
    this->hashes = new unsigned char[num_hashes * num_bytes];
    this->indeces = new int[num_hashes];
    this->result_indeces = new int[max_results];
    this->result_dists = new double[max_results];
    this->temp_hamming = new int[num_hashes];
    this->temp_bitand = new int[num_hashes];
    memcpy(this->hashes, hashes, num_hashes * num_bytes);
    memcpy(this->indeces, indeces, num_hashes * sizeof(int));
}

HammingHashIndex::~HammingHashIndex() {
    delete [] hashes;
    delete [] indeces;
    delete [] result_indeces;
    delete [] result_dists;
    delete [] temp_hamming;
    delete [] temp_bitand;
}

std::vector<std::pair<double, std::string> > *HammingHashIndex::query_index(const unsigned char *hash, int size) {
    int i, max_valid_ind = -1;
    if (size != num_bytes) {
        printf("%d %d\n", size, num_bytes);
        return NULL;
    }
    place_dist_clear_arrays(result_indeces, result_dists, max_results);
    memset(temp_hamming, 0, sizeof(int) * num_hashes);
    memset(temp_bitand, 0, sizeof(int) * num_hashes);
    hamdist_cmap_lut16(hashes, hash, temp_hamming, num_bytes, num_hashes, 1);
    for (i = 0; i < num_hashes; ++i)
        max_valid_ind = place_dist_in_results(i, temp_hamming[i], result_indeces, result_dists, max_results, max_valid_ind);
    std::vector<std::pair<double, std::string> > *out = new std::vector<std::pair<double, std::string> >(max_valid_ind + 1);
    for (int i = 0; i < max_valid_ind + 1; ++i) {
        (*out)[i].first = result_dists[i];
        (*out)[i].second = labels[indeces[result_indeces[i]]];
    }
    return out;
}

} // namespace Picarus
