#ifndef HAMMING_AUX_H
#define HAMMING_AUX_H
#ifdef __cplusplus 
extern "C" {
#endif
void hamdist_cmap_lut16(const unsigned char *xs, const unsigned char *ys, int *out, const int size, const int num_xs, const int num_ys);
void bitand_cmap_lut16(const unsigned char *xs, const unsigned char *ys, int *out, const int size, const int num_xs, const int num_ys);
#ifdef __cplusplus 
}
#endif
#endif
