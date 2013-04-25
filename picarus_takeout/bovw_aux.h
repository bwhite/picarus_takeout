#ifndef BOVW_AUX
#define BOVW_AUX
#ifdef __cplusplus 
extern "C" {
#endif

void bovw_fast_hist(unsigned int *neighbor_map, unsigned int *bovw, const int height, const int width, const int bins, const int level);
void bovw_fast_sum(unsigned int *bovw_fine, unsigned int *bovw_coarse, const int height_fine, const int width_fine, const int bins);

#ifdef __cplusplus 
}
#endif
#endif
