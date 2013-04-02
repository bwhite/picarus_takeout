#include "Hasher.hpp"
#include <vector>

namespace Picarus {
Hasher::Hasher() {
}

Hasher::~Hasher() {
}

void Hasher::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    std::vector<double> vec;
    std::vector<int> shape;
    ndarray_fromstring(input, size, &vec, &shape);
    int size_out;
     /* NOTE(brandyn): No need to free hash output, only one copy exists inside the hasher */
    unsigned char *out = hash_feature(&vec[0], shape[0], &size_out);
    (*collector)(out, size_out);
}
} // namespace Picarus
