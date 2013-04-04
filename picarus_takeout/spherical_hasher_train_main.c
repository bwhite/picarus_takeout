#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "spherical_hasher_train.h"


void rand_vec(double *vec, int size) {
    int i;
    for (i = 0; i < size; ++i)
        vec[i] = rand() / (double)RAND_MAX;
}

int main() {
    int num_points = 10000;
    int num_pivots = 64;
    int num_dims = 64;
    double eps_m = .1;
    double eps_s = .15;
    int max_iters = 50;
    double *points = malloc(sizeof(double) * num_points * num_dims);
    double *pivots = malloc(sizeof(double) * num_pivots * num_dims);
    double *threshs = malloc(sizeof(double) * num_pivots);
    rand_vec(points, num_points * num_dims);
    rand_vec(pivots, num_pivots * num_dims);
    rand_vec(threshs, num_pivots);
    
    spherical_hasher_train(points, pivots, threshs, num_points, num_dims, num_pivots, eps_m, eps_s, max_iters);

    free(points);
    free(pivots);
    free(threshs);
    return 0;
}
