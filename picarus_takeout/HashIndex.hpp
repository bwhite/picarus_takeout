#ifndef HASH_INDEX
#define HASH_INDEX
#include "Model.hpp"

namespace Picarus {
class HashIndex : public Model {
public:
    HashIndex();
    virtual ~HashIndex();
    virtual std::vector<std::pair<double, std::string> > *query_index(const unsigned char *hash, int size) = 0;
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};

} // namespace Picarus
#endif
