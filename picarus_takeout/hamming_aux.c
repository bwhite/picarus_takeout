#include "hamming_aux.h"
#include <stdint.h>

static unsigned char LUT16BIT[65536];
static int LUT16BIT_VALID = 0;


void make_lut16bit(unsigned char *lut16bit) {
    int i, val;
    for (i = 0; i < 65536; ++i) {
        val = i;
        while(val) {
            ++lut16bit[i]; 
            val &= val - 1;
        }
    }  
}

void make_lut16bit_global() {
    if (!LUT16BIT_VALID) {
        make_lut16bit(LUT16BIT);
        LUT16BIT_VALID = 1;
    }
}

void hamdist_cmap_lut16_even(const uint16_t *xs, const uint16_t *ys, int *out, const int size_by_2, const int num_xs, const int num_ys) {
    int i, j, k;
    uint16_t *ys_orig = ys;
    make_lut16bit_global();
    for (i = 0; i < num_xs; ++i, xs += size_by_2)
        for (j = 0, ys = ys_orig; j < num_ys; ++j, ++out, ys += size_by_2) {
            for (k = 0; k < size_by_2; ++k) {
                *out += LUT16BIT[xs[k] ^ ys[k]];
            }
        }
}

void hamdist_cmap_lut16_odd(const unsigned char *xs, const unsigned char *ys, int *out, const int size_by_2, const int num_xs, const int num_ys) {
    int i, j, k;
    int size = size_by_2 * 2;
    int size_p1 = size + 1;
    const unsigned char *ys_orig = ys;
    make_lut16bit_global();
    for (i = 0; i < num_xs; ++i, xs += size_p1)
        for (j = 0, ys = ys_orig; j < num_ys; ++j, ++out, ys += size_p1) {
            for (k = 0; k < size_by_2; ++k) {
                *out += LUT16BIT[((const uint16_t *) xs)[k] ^ ((const uint16_t *) ys)[k]];
            }
            /* NOTE(brandyn): This covers the last byte */
            *out += LUT16BIT[xs[size] ^ ys[size]];
        }
}

void hamdist_cmap_lut16(const unsigned char *xs, const unsigned char *ys, int *out, const int size, const int num_xs, const int num_ys) {
    if (size % 2)
        hamdist_cmap_lut16_odd(xs, ys, out, size / 2, num_xs, num_ys);
    else
        hamdist_cmap_lut16_even((const uint16_t *)xs, (const uint16_t *)ys, out, size / 2, num_xs, num_ys);
}

void bitand_cmap_lut16_even(const uint16_t *xs, const uint16_t *ys, int *out, const int size_by_2, const int num_xs, const int num_ys) {
    int i, j, k;
    const uint16_t *ys_orig = ys;
    make_lut16bit_global();
    for (i = 0; i < num_xs; ++i, xs += size_by_2)
        for (j = 0, ys = ys_orig; j < num_ys; ++j, ++out, ys += size_by_2) {
            for (k = 0; k < size_by_2; ++k) {
                *out += LUT16BIT[xs[k] & ys[k]];
            }
        }
}

void bitand_cmap_lut16_odd(const unsigned char *xs, const unsigned char *ys, int *out, const int size_by_2, const int num_xs, const int num_ys) {
    int i, j, k;
    int size = size_by_2 * 2;
    int size_p1 = size + 1;
    const unsigned char *ys_orig = ys;
    make_lut16bit_global();
    for (i = 0; i < num_xs; ++i, xs += size_p1)
        for (j = 0, ys = ys_orig; j < num_ys; ++j, ++out, ys += size_p1) {
            for (k = 0; k < size_by_2; ++k) {
                *out += LUT16BIT[((const uint16_t *) xs)[k] & ((const uint16_t *) ys)[k]];
            }
            /* NOTE(brandyn): This covers the last byte */
            *out += LUT16BIT[xs[size] & ys[size]];
        }
}

void bitand_cmap_lut16(const unsigned char *xs, const unsigned char *ys, int *out, const int size, const int num_xs, const int num_ys) {
    if (size % 2)
        bitand_cmap_lut16_odd(xs, ys, out, size / 2, num_xs, num_ys);
    else
        bitand_cmap_lut16_even((const uint16_t *)xs, (const uint16_t *)ys, out, size / 2, num_xs, num_ys);
}
