#ifndef HAMMING_FEATURE2D_HASH_INDEX
#define HAMMING_FEATURE2D_HASH_INDEX
#include "Model.hpp"
#include <string>
#include <vector>
#include "HammingMultiIndex.hpp"

namespace Picarus {
class HammingFeature2dHashIndex : public Model {
private:
    const bool mih;
    const int num_hashes;
    const int num_bytes;
    const std::vector<std::string> labels;
    const int max_results;
    const int max_keypoint_results;
    const int hamming_thresh;
    unsigned char *hashes;
    //double *keypoints;
    int *indeces;
    int *result_indeces;
    double *result_dists;
    double *idf;
    int *temp_hamming;
    int *temp_bitand;
    HammingMultiIndex *index;
public:
    HammingFeature2dHashIndex(const unsigned char *hashes, int *indeces, int num_hashes, int num_bytes,
                              const std::vector<std::string> &labels, int max_results, int max_keypoint_results, int hamming_thresh);
    virtual ~HammingFeature2dHashIndex();
    virtual std::vector<std::pair<double, std::string> > *query_index(const unsigned char *hashes, double *keypoints, int num_hashes, int num_bytes);
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};

} // namespace Picarus
#endif
