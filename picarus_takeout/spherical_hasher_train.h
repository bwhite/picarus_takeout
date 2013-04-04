#ifndef SPHERICAL_HASHER_TRAIN
#define SPHERICAL_HASHER_TRAIN
#ifdef __cplusplus 
extern "C" {
#endif
void spherical_hasher_train(const double *points, double *pivots, double *threshs, const int num_points, const int num_dims, const int num_pivots,
                            const double eps_m, const double eps_s, const int max_iters);
#ifdef __cplusplus 
}
#endif
#endif
