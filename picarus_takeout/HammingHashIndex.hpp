#ifndef HAMMING_HASH_INDEX
#define HAMMING_HASH_INDEX
#include "HashIndex.hpp"
#include <string>
#include <vector>

namespace Picarus {
class HammingHashIndex : public HashIndex {
private:
    const int num_hashes;
    const int num_bytes;
    const int max_results;
    unsigned char *hashes;
    int *indeces;
    const std::vector<std::string> labels;
    int *result_indeces;
    double *result_dists;
    int *temp_hamming;
    int *temp_bitand;
public:
    HammingHashIndex(const unsigned char *hashes, int *indeces, int num_hashes, int num_bytes, const std::vector<std::string> &labels, int max_results);
    virtual ~HammingHashIndex();
    virtual std::vector<std::pair<double, std::string> > *query_index(const unsigned char *hash, int size);
};

} // namespace Picarus
#endif
