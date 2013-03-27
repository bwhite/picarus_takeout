#ifndef GIST_WRAPPER_H
#define GIST_WRAPPER_H
#ifdef __cplusplus 
extern "C" {
#endif
void color_gist_scaletab_wrap(unsigned char *data, int height, int width, int nblocks, int n_scale, const int *orientations_per_scale, float *desc, int desc_size);
#ifdef __cplusplus 
}
#endif
#endif
