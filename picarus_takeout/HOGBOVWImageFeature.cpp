#include "opencv_helpers.hpp"
#include "HistogramImageFeature.hpp"
#include "pyramid_histogram_aux.h"

namespace Picarus {

HOGBOVWImageFeature::HOGBOVWImageFeature(std::string mode, std::vector<int> num_bins, int levels) : mode(mode), num_bins(num_bins), levels(levels) {
    color_mode_to_code(mode, min_vals, max_vals, &skip_cvt_color, &code);
}

HOGBOVWImageFeature::~HOGBOVWImageFeature() {
}


unsigned int *HOGBOVWImageFeature::histogram_label_image(unsigned char *image, int height, int width, int *out_max_val) {
    cv::Mat image_mat(height, width, CV_8UC3, image);
    cv::Mat image_matf;
    cv::Mat image_matf_color(height, width, CV_32FC3);

    unsigned int *bin_map = new unsigned int[height * width];
    float *image_color = convert_color(image, height, width, code, skip_cvt_color);
    *out_max_val = num_bins[0] * num_bins[1] * num_bins[2];
    float bin_width[3];
    for (int i = 0; i < 3; ++i)
        bin_width[i] = (max_vals[i] - min_vals[i]) / num_bins[i];
    image_to_bin_map(image_color, height, width, min_vals, bin_width, &num_bins[0], bin_map);
    delete [] image_color;
    return bin_map;
}

double *HOGBOVWImageFeature::compute_feature(unsigned char *image, int height, int width, int *out_size) {
    int max_val;
    unsigned int *label_image = histogram_label_image(image, height, width, &max_val);
    double *out = pyramid_histogram(label_image, height, width, max_val, levels, out_size);
    delete [] label_image;
    return out;
}
} // namespace Picarus
