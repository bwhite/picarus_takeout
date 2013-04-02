#ifndef HASHER
#define HASHER
#include "Model.hpp"

namespace Picarus {
class Hasher : public Model {
public:
    Hasher();
    virtual ~Hasher();
    virtual unsigned char *hash_feature(double *feature, int size, int *size_out) = 0;
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};

} // namespace Picarus
#endif
