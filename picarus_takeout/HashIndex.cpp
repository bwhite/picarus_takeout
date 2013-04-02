#include "HashIndex.hpp"
#include <vector>

namespace Picarus {
HashIndex::HashIndex() {
}

HashIndex::~HashIndex() {
}

void HashIndex::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    std::vector<std::pair<double, std::string> > *out = query_index(input, size);
    if (out == NULL)
        (*collector)(NULL, 0);
    else {
        double_strings_tostring(*out, collector);
        delete out;
    }
}
} // namespace Picarus
