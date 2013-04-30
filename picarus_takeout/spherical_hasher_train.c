#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "spherical_hasher_train.h"

typedef struct {
    int index;
    double value;
} dist_t;

static int dist_cmp(const void *a, const void *b) { 
    const double av = ((dist_t*)a)->value;
    const double bv = ((dist_t*)b)->value;
    if (av > bv)
        return 1;
    if (av < bv)
        return -1;
    return 0;
}

void spherical_hasher_train(const double *points, double *pivots, double *threshs, const int num_points, const int num_dims, const int num_pivots,
                            const double eps_m, const double eps_s, const int max_iters) {
    const double m_div_4 = num_points / 4.;
    const int num_points_div_2 = num_points / 2;
    const double force_scale = 1. / (m_div_4 * 2.) / num_pivots;
    const double force_shift = 1. / (num_pivots * 2.);
    const double eps_m_scaled = eps_m * m_div_4;
    const double eps_s_scaled = eps_s * m_div_4;
    const double eps_v_scaled = eps_s_scaled * eps_s_scaled;
    unsigned char *memberships = malloc(num_pivots * num_points); // #pivots x #points
    int *cooccurrences = malloc(sizeof(int) * num_pivots * num_pivots); // #pivots x #pivots
    double *forces = malloc(sizeof(double) * num_pivots * num_dims); // #pivots x #dims
    dist_t *dists = malloc(sizeof(dist_t) * num_points); // #points
    int i, j, k, l, cnt;
    double v, c_abs_shift_sum, c_sum, c_sqr_sum, c_mean, c_var, mean_ratio, var_ratio;
    for (i = 0; i < max_iters; ++i) {
        memset(memberships, 0, num_pivots * num_points);
        /* Compute memberships */
        for (j = 0; j < num_pivots; ++j) {
            /* Compute pivot to point distances */
            for (k = 0; k < num_points; ++k) {
                v = 0.;
                for (l = 0; l < num_dims; ++l)
                    v += (points[k * num_dims + l] - pivots[j * num_dims + l]) * (points[k * num_dims + l] - pivots[j * num_dims + l]);
                dists[k].index = k;
                dists[k].value = v;
            }
            /* Compute median threshold */
            qsort(dists, num_points, sizeof(dist_t), dist_cmp);
            threshs[j] = dists[num_points_div_2].value;
            // NOTE(brandyn): Assumes that the distances are unlikely to be equal
            // could miss those who have equal distance but less than the median position
            for (k = num_points_div_2; k < num_points; ++k)
                memberships[j * num_points + dists[k].index] = 1;                
        }
        /* Fill out upper triangle only as it is symmetric */
        memset(cooccurrences, 0, sizeof(int) * num_pivots * num_pivots);
        for (j = 0; j < num_pivots - 1; ++j)
            for (k = 0; k < num_points; ++k)
                if (memberships[j * num_points + k])
                    for (l = j + 1; l < num_pivots; ++l)
                        if (memberships[l * num_points + k])
                            ++cooccurrences[j * num_pivots + l];
        /* Compute stopping conditions */
        c_abs_shift_sum = 0.;
        c_sum = 0.;
        c_sqr_sum = 0.;
        cnt = 0;
        for (j = 0; j < num_pivots - 1; ++j)
            for (k = j + 1; k < num_pivots; ++k) {
                c_abs_shift_sum += abs(cooccurrences[j * num_pivots + k] - m_div_4);
                c_sum += cooccurrences[j * num_pivots + k];
                c_sqr_sum += cooccurrences[j * num_pivots + k] * cooccurrences[j * num_pivots + k];
                ++cnt;
            }
        c_mean = c_abs_shift_sum / cnt;
        c_var = (c_sqr_sum - c_sum * c_sum / cnt) / cnt;
        mean_ratio = c_mean / eps_m_scaled;
        var_ratio = c_var / eps_v_scaled;
        printf("%d %f %f\n", i, mean_ratio, var_ratio);
        if (mean_ratio <= 1 && var_ratio <= 1)
            break;
        /* Update pivots */
        memset(forces, 0, sizeof(double) * num_pivots * num_dims);
        for (j = 0; j < num_pivots - 1; ++j)
            for (k = j + 1; k < num_pivots; ++k) {
                double f = force_scale * cooccurrences[j * num_pivots + k] - force_shift;
                //double f = (cooccurrences[j * num_pivots + k] - m_div_4) / (m_div_4 * 2 * num_points);
                for (l = 0; l < num_dims; ++l) {
                    double f_cur = (pivots[j * num_dims + l] - pivots[k * num_dims + l]) * f;
                    forces[j * num_dims + l] += f_cur;
                    forces[k * num_dims + l] -= f_cur;
                }
            }
        for (j = 0; j < num_pivots; ++j)
            for (k = 0; k < num_dims; ++k)
                pivots[j * num_dims + k] += forces[j * num_dims + k];
    }
    free(memberships);
    free(cooccurrences);
    free(forces);
    free(dists);
}
