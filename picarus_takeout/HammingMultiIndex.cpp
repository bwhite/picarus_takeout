#include "HammingMultiIndex.hpp"
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace Picarus {
HammingMultiIndex::HammingMultiIndex(const unsigned char *hashes, int num_hashes, int num_bytes,
                                     int max_results) : num_hashes(num_hashes), num_bytes(num_bytes), num_bits(num_bytes * 8), max_results(max_results) {
    db = new unsigned char[num_hashes * num_bytes];
    memcpy(db, hashes, num_hashes * num_bytes);
    m = optimal_m(num_hashes, num_bits);
    printf("m[%d]\n", m);
    index = create_index(db, num_hashes, num_bits, m);
}

HammingMultiIndex::~HammingMultiIndex() {
    free_index(index, num_hashes, num_bits, m);
    delete [] db;
}

int *HammingMultiIndex::query_index(const unsigned char *hash, int num_bytes, int *size_out) {
    uint32_t *results = NULL;
    int results_size = 0;
    int results_capacity = 0;
    search_index_k(db, index, num_hashes, num_bits, m, (uint8_t *)hash, max_results, &results, &results_size, &results_capacity);
    int *results_out = new int[results_size];
    printf("%d %d\n", results_size, results_capacity);
    for (int i = 0; i < results_size; ++i) {
        printf("%d ", results[i]);
        results_out[i] = results[i];
    }
    printf("\n");
    free(results);
    *size_out = results_size / 2;
    return results_out;
}
}
