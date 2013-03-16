#ifndef HISTOGRAM_AUX
#define HISTOGRAM_AUX
#ifdef __cplusplus 
extern "C" {
    void image_to_bin_map(float *data, int height, int width, float *min_vals, float *bin_width, int *num_bins, unsigned int *bin_map);
}
#endif
void image_to_bin_map(float *data, int height, int width, float *min_vals, float *bin_width, int *num_bins, unsigned int *bin_map);
#endif
