#ifndef OPENCV_HELPERS
#define OPENCV_HELPERS
#include <opencv2/core/core_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <string>

void color_mode_to_code(std::string mode, float *min_vals, float *max_vals, int *skip_cvt_color, int *code);
float *convert_color(unsigned char *image, int height, int width, int code, int skip_cvt_color);
unsigned char *bgr_to_rgb(unsigned char *image, int height, int width);
void scale_image(float *image, int height, int width, float *min_vals, float *max_vals);
double *pyramid_histogram(unsigned int *label_image, int height, int width, int max_val, int levels, int *out_size);
#endif
