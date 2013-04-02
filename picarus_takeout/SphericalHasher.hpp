#ifndef SPHERICAL_HASHER
#define SPHERICAL_HASHER
#include "Hasher.hpp"

namespace Picarus {
class SphericalHasher : public Hasher {
private:
    double *pivots;
    double *threshs;
    unsigned char *hash;
    const int num_pivots;
    const int num_dims;
    const int num_bytes;
public:
    SphericalHasher(double *pivots, double *threshs, int num_pivots, int num_dims);
    virtual ~SphericalHasher();
    virtual unsigned char *hash_feature(double *feature, int size, int *size_out);
};

} // namespace Picarus
#endif
