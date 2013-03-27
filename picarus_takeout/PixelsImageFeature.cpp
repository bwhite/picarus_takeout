#include "opencv_helpers.hpp"
#include "PixelsImageFeature.hpp"
#include "pyramid_histogram_aux.h"

namespace Picarus {

PixelsImageFeature::PixelsImageFeature(std::string mode) : mode(mode) {
    color_mode_to_code(mode, min_vals, max_vals, &skip_cvt_color, &code);
}

PixelsImageFeature::~PixelsImageFeature() {
}

double *PixelsImageFeature::compute_feature(unsigned char *image, int height, int width, int *out_size) {
    float *image_color_f = convert_color(image, height, width, code, skip_cvt_color);
    scale_image(image_color_f, height, width, min_vals, max_vals);
    int size = height * width * 3;
    double *image_color_d = new double[size];
    for (int i = 0; i < size; ++i)
        image_color_d[i] = image_color_f[i];
    delete [] image_color_f;
    *out_size = size;
    return image_color_d;
}
} // namespace Picarus
