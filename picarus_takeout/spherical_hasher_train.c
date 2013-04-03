#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    int index;
    double value;
} dist_t;

static int dist_cmp(const void *a, const void *b) { 
    return ((dist_t*)a)->value - ((dist_t*)b)->value;
}

static void pivot_dist(const double *points, const double *pivots, dist_t *dists, const int num_points, const int num_dims, const int num_pivots) {
    int i, j;
    double v;
    for (i = 0; i < num_points; ++i) {
        v = 0.;
        for (j = 0; j < num_dims; ++j)
            v += (points[i * num_dims + j] - pivots[j]) * (points[i * num_dims + j] - pivots[j]);
        dists[i].index = i;
        dists[i].value = v;
    }
}

void spherical_hasher_train(const double *points, double *pivots, double *threshs, const int num_points, const int num_dims, const int num_pivots,
                            const double eps_m, const double eps_s, const int max_iters) {
    const double m_div_4 = num_points / 4.;
    const int num_points_div_2 = num_points / 2;
    const double force_scale = .5 / m_div_4 / num_pivots;
    const double force_shift = .5 / num_pivots;
    const double eps_m_scaled = eps_m * m_div_4;
    const double eps_s_scaled = eps_s * m_div_4;
    const double eps_v_scaled = eps_s_scaled * eps_s_scaled;
    unsigned char *memberships = malloc(num_pivots * num_points); // #pivots x #points
    int *cooccurrences = malloc(sizeof(int) * num_pivots * num_pivots); // #pivots x #pivots
    double *forces = malloc(sizeof(double) * num_pivots * num_dims); // #pivots x #dims
    dist_t *dists = malloc(sizeof(dist_t) * num_points); // #points
    int i, j, k, l;
    for (i = 0; i < max_iters; ++i) {
        memset(memberships, 0, num_pivots * num_points);
        for (j = 0; j < num_pivots; ++j) {
            pivot_dist(points, pivots, dists, num_points, num_dims, num_pivots);
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
                    for (l = j; l < num_pivots; ++l)
                        if (memberships[l * num_points + k])
                            ++cooccurrences[j * num_pivots + l];
        /* Compute stopping conditions */
        double c_abs_shift_sum = 0.;
        double c_sum = 0.;
        double c_sqr_sum = 0.;
        int cnt = 0;
        for (j = 0; j < num_pivots - 1; ++j)
            for (k = j; k < num_pivots; ++k) {
                c_abs_shift_sum += abs(cooccurrences[j * num_pivots + k] - m_div_4);
                c_sum += cooccurrences[j * num_pivots + k];
                c_sqr_sum += cooccurrences[j * num_pivots + k] * cooccurrences[j * num_pivots + k];
                ++cnt;
            }
        double c_mean = c_abs_shift_sum / cnt;
        double c_var = (cnt * c_sqr_sum - c_sum * c_sum) / (double)(cnt * (cnt - 1));
        double mean_ratio = c_mean / eps_m_scaled;
        double var_ratio = c_var / eps_v_scaled;
        printf("%d %f %f\n", i, mean_ratio, var_ratio);
        if (mean_ratio <= 1 && var_ratio <= 1)
            break;
        /* Update pivots */
        memset(forces, 0, sizeof(double) * num_pivots * num_dims);
        for (j = 0; j < num_pivots - 1; ++j)
            for (k = j + 1; k < num_pivots; ++k) {
                double f = force_scale * cooccurrences[j * num_pivots + k] - force_shift;
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
