#ifndef MULTI_INDEX_H
#define MULTI_INDEX_H
#ifdef __cplusplus 
extern "C" {
#endif
#include <stdint.h>
void search_index_r(uint32_t ***index, int n, int b, int m, uint8_t *query, int r, uint32_t **results, int *results_size, int *results_capacity, uint8_t *results_mask);
uint32_t ***create_index(uint8_t *db, const int n, const int b, const int m);
void free_index(uint32_t ***index, int n, int b, int m);
int optimal_m(const int n, const int b);
void unique_results(uint32_t **results, int *results_size, int *results_capacity);
void search_index_k(uint8_t *db, uint32_t ***index, int n, int b, int m, uint8_t *query, int k, uint32_t **results, int *results_size, int *results_capacity);
int uint32_t_pair_cmp(const void *a, const void *b);
#ifdef __cplusplus 
}
#endif
#endif
