#ifndef HAMMING_MULTI_INDEX
#define HAMMING_MULTI_INDEX
#include "multi_index.h"

namespace Picarus {
class HammingMultiIndex {
private:
    const int num_hashes;
    const int num_bytes;
    const int num_bits;
    const int max_results;
    uint32_t ***index;
    uint8_t *db;
    int m;
    uint32_t **results;
public:
    HammingMultiIndex(const unsigned char *hashes, int num_hashes, int num_bytes, int max_results);
    virtual ~HammingMultiIndex();
    int *query_index(const unsigned char *hash, int num_bytes, int *size_out);
};
} // namespace Picarus
#endif
