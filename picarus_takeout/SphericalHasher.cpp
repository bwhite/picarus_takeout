#include "SphericalHasher.hpp"
#include <cmath>
#include <cstring>

namespace Picarus {
SphericalHasher::SphericalHasher(double *pivots, double *threshs, int num_pivots, int num_dims) : num_pivots(num_pivots), num_dims(num_dims), num_bytes(ceil(num_dims / 8.)) {
    this->pivots = new double[num_pivots * num_dims];
    this->threshs = new double[num_pivots];
    this->hash = new unsigned char[num_bytes];
    memcpy(this->pivots, pivots, sizeof(double) * num_pivots * num_dims);
    memcpy(this->threshs, threshs, sizeof(double) * num_pivots);
}

SphericalHasher::~SphericalHasher() {
    delete [] pivots;
    delete [] threshs;
    delete [] hash;
}

unsigned char *SphericalHasher::hash_feature(double *feature, int size, int *size_out) {
    memset(hash, 0, num_bytes);
    for (int i = 0; i < num_pivots; ++i) {
        double distSqr = 0.;
        for (int j = 0; j < num_dims; ++j)
            distSqr += (feature[j] - pivots[i * num_dims + j]) * (feature[j] - pivots[i * num_dims + j]);
        if (distSqr <= threshs[i])
            hash[i / 8] |= 1 << (i % 8);
    }
    return hash;
}
} // namespace Picarus
