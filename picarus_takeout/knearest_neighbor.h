#ifndef KNEAREST_NEIGHBOR_H
#define KNEAREST_NEIGHBOR_H
#ifdef __cplusplus 
extern "C" {
#endif
int knnl2sqr(double *test_point, double *train_points, int *neighbor_indeces, double *neighbor_dists, int num_train_points, int num_dims, int num_neighbors);
void nnsl2sqr(double *test_points, double *train_points, unsigned int *neighbor_indeces, double *neighbor_distances, unsigned int num_test_points, unsigned int num_train_points, unsigned int num_dims);
int place_dist_in_results(const int index, const double dist, int *neighbor_indeces, double *neighbor_dists, const int num_neighbors, const int max_valid_ind);
#ifdef __cplusplus 
}
#endif
#endif
