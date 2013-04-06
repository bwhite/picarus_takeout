/* Copyright 2012 Brandyn White <bwhite@dappervision.com> - Apache 2.0 */
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "multi_index.h"
#include "hamming_aux.h"

uint32_t extract_bits(uint8_t *db, const int s, const int j) {
    uint64_t out = 0;
    const int js = j * s;
    const int front_excess = js % 8;
    const int num_bytes = ceil((front_excess + s) / 8.);    
    const int back_excess = 64 - (front_excess + s);
    memcpy(&out, db + js / 8, num_bytes);
    return (out << back_excess) >> (front_excess + back_excess);
}

uint32_t extract_bits_part(uint8_t *db, const int s, const int s_part, const int j) {
    uint64_t out = 0;
    const int js = j * s;
    const int front_excess = js % 8;
    const int num_bytes = ceil((front_excess + s_part) / 8.);    
    const int back_excess = 64 - (front_excess + s_part);
    memcpy(&out, db + js / 8, num_bytes);
    return (out << back_excess) >> (front_excess + back_excess);
}

int optimal_m(const int n, const int b) {
    int m = round(b / log2(n));  /* s = ceil(b / (double)m) */
    m = m >= 1 ? m : 1;
    m = m >= b ? b : m;
    int s = ceil(b / (double)m);
    int m_full = b / s;  /* Number of whole hash tables (s bits) */
    int s_part = b - m_full * s;  /* Number of bits in partial table */
    if (s < 32 && m == m_full + (s_part > 0))
        return m;
    ++m;
    while (m < b) {
        s = ceil(b / (double)m);
        m_full = b / s;  /* Number of whole hash tables (s bits) */
        s_part = b - m_full * s;  /* Number of bits in partial table */
        if (s < 32 && m == m_full + (s_part > 0))
            return m;
        ++m;
    }
    assert(0); /* Failure case: Couldnt find an optimal m value */
}

uint32_t ***create_index(uint8_t *db, const int n, const int b, const int m) {
    /*
      Args:
          n: (num_values): 
          b: (num_bits):
          m: (num_indexes):
     */
    uint32_t i, j, k;
    assert(n > 0);  /* Sanity */
    assert(b > 0);  /* Sanity */
    assert(m > 0);  /* Sanity */
    assert(b % 8 == 0);  /* Divisible by 8 */
    assert(m <= b);  /* At least as many bits than indexes */
    assert(b / m < 32);  /* Need to rewrite part of code to handle >= 32 */
    /* Begin: Compute parameters */
    const uint32_t s = ceil(b / (double)m);  /* Max bits per hash table*/
    const int b_by_8 = b / 8;
    const int m_full = b / s;  /* Number of whole hash tables (s bits) */
    const int s_part = b - m_full * s;  /* Number of bits in partial table */
    const uint32_t num_buckets = ((uint32_t) 1) << s;
    const uint32_t num_buckets_part = ((uint32_t) 1) << s_part;
    const int sz = n * b_by_8;
    /* End: Compute parameters */
    printf("n[%d] b[%d] m[%d] m_full[%d] s[%d] s_part[%d]\n", n, b, m , m_full, s, s_part);
    assert(ceil(s / 8.) <= 4); /* Need to rewrite to handle */
    assert(b == m_full * s + s_part);  /* All bits are covered */
    assert(m == m_full + (s_part > 0));  /* No unused indexes */
    uint32_t **cnts = malloc(m * sizeof(uint32_t*)); /* [m][2 ** s] */
    if (cnts == NULL) return NULL;
    uint32_t ***index = malloc(m * sizeof(uint32_t**));
    if (index == NULL) return NULL;
    /* Find much space we need by passing through the db once */
    for (i = 0; i < m_full; ++i) {
        cnts[i] = calloc(num_buckets, sizeof(uint32_t));
        if (cnts[i] == NULL) return NULL;
    }
    if (s_part) {
        cnts[m_full] = calloc(num_buckets_part, sizeof(uint32_t));
        if (cnts[m_full] == NULL) return NULL;
    }
    for (i = 0; i < sz; i += b_by_8) {
        for (j = 0; j < m_full; ++j) {
            ++cnts[j][extract_bits(db + i, s, j)];
        }
        if (s_part) {
            ++cnts[m_full][extract_bits_part(db + i, s, s_part, j)];
        }
    }
    /* Allocate enough space for the index */
    for (i = 0; i < m_full; ++i) {
        index[i] = malloc(num_buckets * sizeof(uint32_t*));
        if (index[i] == NULL) return NULL;
        for (j = 0; j < num_buckets; ++j) {
            index[i][j] = malloc((cnts[i][j] + 1) * sizeof(uint32_t));
            if (index[i][j] == NULL) return NULL;
            index[i][j][cnts[i][j]] = UINT32_MAX;
        }
    }
    if (s_part) {
        index[m_full] = malloc(num_buckets_part * sizeof(uint32_t*));
        if (index[m_full] == NULL) return NULL;
        for (j = 0; j < num_buckets_part; ++j) {
            index[m_full][j] = malloc((cnts[m_full][j] + 1) * sizeof(uint32_t));
            if (index[m_full][j] == NULL) return NULL;
            index[m_full][j][cnts[m_full][j]] = UINT32_MAX;
        }
    }
    for (i = 0, k = 0; i < sz; i += b_by_8, ++k) {
        for (j = 0; j < m_full; ++j) {
            uint32_t bucket = extract_bits(db + i, s, j);
            index[j][bucket][--cnts[j][bucket]] = k;
        }
        if (s_part) {
            uint32_t bucket = extract_bits_part(db + i, s, s_part, m_full);
            index[m_full][bucket][--cnts[m_full][bucket]] = k;
        }
    }
    /* Free up temporary space */
    for (i = 0; i < m; ++i)
        free(cnts[i]);
    free(cnts);
    return index;
}

void free_index(uint32_t ***index, int n, int b, int m) {
    int i, j;
    /* Begin: Compute parameters */
    const uint32_t s = ceil(b / (double)m);  /* Max bits per hash table*/
    const int m_full = b / s;  /* Number of whole hash tables (s bits) */
    const int s_part = b - m_full * s;  /* Number of bits in partial table */
    const uint32_t num_buckets = ((uint32_t) 1) << s;
    const uint32_t num_buckets_part = ((uint32_t) 1) << s_part;
    /* End: Compute parameters */
    for (i = 0; i < m_full; ++i) {
        for (j = 0; j < num_buckets; ++j) {
            free(index[i][j]);
        }
        free(index[i]);
    }
    if (s_part) {
        for (j = 0; j < num_buckets_part; ++j) {
            free(index[m_full][j]);
        }
        free(index[m_full]);
    }
    free(index);
}

void append(uint32_t **results, int *size, int *capacity, uint32_t value) {
    if (*size >= *capacity) {
        *capacity = *capacity > 0 ? 2 * (*capacity) : 64;
        *results = realloc(*results, *capacity * sizeof(uint32_t));
        assert(*results != NULL);
    }
    (*results)[(*size)++] = value;
}

void combinations_non_recursive(int n, int k, uint32_t bucket,
                                uint32_t **index, uint32_t **results, int *results_size, int *results_capacity, uint8_t *results_mask) {
    uint32_t *stack = NULL;
    uint32_t *h = NULL;
    int stack_size = 0;
    int stack_capacity = 0;
    int i, j, l;
    /* Initial */
    if (n < k)
        return;
    append(&stack, &stack_size, &stack_capacity, k);
    append(&stack, &stack_size, &stack_capacity, bucket);
    append(&stack, &stack_size, &stack_capacity, 0);

    /* Steady state */
    while (stack_size > 0) {
        /* h[0] = k, h[1] = bucket, h[2] = i */
        h = stack + stack_size - 3;
        k = h[0];
        bucket = h[1];
        i = h[2];
        stack_size -= 3;
        if (!k) {
            for (j = 0; index[bucket][j] != UINT32_MAX; ++j) {
                l = index[bucket][j];
                if (!(results_mask[l / 8] & 1 << l % 8)) {
                    append(results, results_size, results_capacity, l);
                    results_mask[l / 8] |= 1 << l % 8;
                }
            }
            continue;
        }
        /* Add new elements */
        if (n >= (k - 1) + (i + 1)) {
            append(&stack, &stack_size, &stack_capacity, k - 1);
            append(&stack, &stack_size, &stack_capacity, bucket ^ (((uint32_t) 1) << h[2]));
            append(&stack, &stack_size, &stack_capacity, i + 1);
        }
        if (n >= k + (i + 1)) {  /* If the previous is true then this isn't either, avoids another condition */
            append(&stack, &stack_size, &stack_capacity, k);
            append(&stack, &stack_size, &stack_capacity, bucket);
            append(&stack, &stack_size, &stack_capacity, i + 1);
        }
    }
    free(stack);
}

void search_index_r(uint32_t ***index, int n, int b, int m, uint8_t *query, int r, uint32_t **results, int *results_size, int *results_capacity, uint8_t *results_mask) {
    int i;
    uint32_t bucket;
    /* Begin: Compute parameters */
    const uint32_t s = ceil(b / (double)m);  /* Max bits per hash table*/
    const int m_full = b / s;  /* Number of whole hash tables (s bits) */
    const int s_part = b - m_full * s;  /* Number of bits in partial table */
    /* End: Compute parameters */
    assert(r >= 0);
    r = r <= s ? r : s;  /* min(r, s) */
    int r_part = r <= s_part ? r : s_part;  /* min(r_part, s_part) */
    for (i = 0; i < m_full; ++i) {
        bucket = extract_bits(query, s, i);
        combinations_non_recursive(s, r, bucket, index[i], results, results_size, results_capacity, results_mask);
    }
    if (s_part) {
        bucket = extract_bits_part(query, s, s_part, m_full);
        combinations_non_recursive(s_part, r_part, bucket, index[m_full], results, results_size, results_capacity, results_mask);
    }
}

int uint32_t_pair_cmp(const void *a, const void *b) {
    /* NOTE: This limits the effective number of image id bits to 31 */
    int32_t d0 = (((int32_t*)a)[0] - ((int32_t*)b)[0]); /* First is hamming distance, it won't fill up the 31 bits */
    if (d0)
        return d0;
    return ((int32_t*)a)[1] - ((int32_t*)b)[1];
}

void truncate_results(uint8_t *db, const int n, const int b, const int m, uint8_t *query, int k, uint32_t **results, int *results_size, int *results_capacity) {
    /* Truncate results at k values, sorting ascending by distance to the query.  Results includes distance and index  */
    int i;
    const int b_by_8 = b / 8;
    uint32_t dist;
    uint32_t *new_results = NULL;
    const int size = *results_size;
    *results_capacity = 0;
    *results_size = 0;
    k = k <= size ? k : size;
    for (i = 0; i < size; ++i) {
        dist = 0;
        hamdist_cmap_lut16(db + (*results)[i] * b_by_8, query, &dist, b_by_8, 1, 1);
        append(&new_results, results_size, results_capacity, dist);
        append(&new_results, results_size, results_capacity, (*results)[i]);
    }
    if (*results_size)
        qsort(new_results, size, sizeof(uint32_t) * 2, uint32_t_pair_cmp);
    /* Swap results buffers, free previous and truncate */
    free(*results);
    *results = new_results;
    *results_size = k * 2;
}

int count_results(uint8_t *db, const int n, const int b, const int m, uint8_t *query, int r, uint32_t **results, int *results_size, int *results_capacity) {
    /* Counts the number of results that are with distance "r" to the query */
    int i;
    const int b_by_8 = b / 8;
    uint32_t dist;
    int count = 0;
    const int size = *results_size;
    for (i = 0; i < size; ++i) {
        dist = 0;
        hamdist_cmap_lut16(db + (*results)[i] * b_by_8, query, &dist, b_by_8, 1, 1);
        if (dist <= r)
            ++count;
    }
    return count;
}

void search_index_k(uint8_t *db, uint32_t ***index, int n, int b, int m, uint8_t *query, int k, uint32_t **results, int *results_size, int *results_capacity) {
    int r = 0;
    int count = 0;
    uint8_t *results_mask = calloc(ceil(n / 8.), sizeof(uint8_t));
    k = k <= n ? k : n;
    assert(*results_size == 0);
    while (r <= b) {
        search_index_r(index, n, b, m, query, r, results, results_size, results_capacity, results_mask);
        printf("Results[r=%d][%d]\n", r, *results_size);
        count = count_results(db, n, b, m, query, (r + 1) * m - 1, results, results_size, results_capacity);
        printf("Count[r=%d][%d] >= [%d]\n", r, count, k);
        if (count >= k) {
            truncate_results(db, n, b, m, query, k, results, results_size, results_capacity);
            break;
        }
        ++r;
    }
    free(results_mask);
}
