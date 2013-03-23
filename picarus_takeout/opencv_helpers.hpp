#ifndef OPENCV_HELPERS
#define OPENCV_HELPERS
#include <opencv2/opencv.hpp>
#include <string>

void color_mode_to_code(std::string mode, float *min_vals, float *max_vals, int *skip_cvt_color, int *code);
float *convert_color(unsigned char *image, int height, int width, int code, int skip_cvt_color);
#endif
