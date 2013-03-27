#include "opencv_helpers.hpp"
#include "bovw_aux.h"

static int buckets_per_side(int level) {
    return 1 << level;
}

static int bins_per_level(int level, int max_val) {
    int b = buckets_per_side(level);
    return b * b * max_val;
}

static void normalize_histogram(unsigned int *hist, double *hist_norm, int start, int stop, double scale) {
    double normalize_sum = 0.;
    for (int i = start; i < stop; ++i)
        normalize_sum += hist[i];
    for (int i = start; i < stop; ++i)
        hist_norm[i] = (hist[i] / normalize_sum) * scale;           
}

double *pyramid_histogram(unsigned int *label_image, int height, int width, int max_val, int levels, int *out_size) {
    // NOTE: max_val means that label_image is [0, max_val)
    int num_bins = 0;
    for (int i = 0; i < levels; ++i)
        num_bins += bins_per_level(i, max_val);
    unsigned int *out = new unsigned int[num_bins];
    memset(out, 0, sizeof(unsigned int) * num_bins);
    double *out_norm = new double[num_bins];
    *out_size = num_bins;
    bovw_fast_hist(label_image, &out[0], height, width, max_val, levels - 1);
    int offset = 0;
    for (int i = 0; i < levels - 1; ++i) {
        int coarse_offset = offset + bins_per_level(levels - i - 1, max_val);
        double scale = 1 << (levels - i);
        normalize_histogram(out, out_norm, offset, coarse_offset, scale);
        bovw_fast_sum(out + offset, out + coarse_offset, buckets_per_side(levels - i - 1), buckets_per_side(levels - i - 1), max_val);
        offset = coarse_offset;
    }
    normalize_histogram(out, out_norm, offset, num_bins, 1.);
    delete [] out;
    return out_norm;
}

unsigned char *bgr_to_rgb(unsigned char *image, int height, int width) {
    cv::Mat image_mat(height, width, CV_8UC3, image);
    unsigned char *image_mat_color_data = new unsigned char[height * width * 3];
    cv::Mat image_mat_color(height, width, CV_8UC3, image_mat_color_data);
    cvtColor(image_mat, image_mat_color, CV_BGR2RGB);
    return image_mat_color_data;
}

void color_mode_to_code(std::string mode, float *min_vals, float *max_vals, int *skip_cvt_color, int *code) {
    for (int i = 0; i < 3; ++i) {
        min_vals[i] = 0.;
        max_vals[i] = 1.;
    }
    *skip_cvt_color = 0;

    if (!mode.compare("rgb")) {
        *code = CV_BGR2RGB;
    } else if (!mode.compare("xyz")) {
        *code = CV_BGR2XYZ;
    } else if (!mode.compare("ycrcb")) {
        *code = CV_BGR2YCrCb;
    } else if (!mode.compare("hsv")) {
        *code = CV_BGR2HSV;
        max_vals[0] = 360;
    } else if (!mode.compare("luv")) {
        *code = CV_BGR2Luv;
        min_vals[1] = -134;
        min_vals[2] = -140;
        max_vals[0] = 100;
        max_vals[1] = 220;
        max_vals[2] = 122;
    } else if (!mode.compare("hls")) {
        *code = CV_BGR2HLS;
        max_vals[0] = 360;
    } else if (!mode.compare("lab")) {
        *code = CV_BGR2Lab;
        min_vals[1] = -127;
        min_vals[2] = -127;
        max_vals[0] = 100;
        max_vals[1] = 127;
        max_vals[2] = 127;
    } else {
        *skip_cvt_color = 1;
    }
}


float *convert_color(unsigned char *image, int height, int width, int code, int skip_cvt_color) {
    // TODO: Test ranges with all BGR color inputs
    cv::Mat image_mat(height, width, CV_8UC3, image);
    float *image_matf_color_data = new float[height * width * 3];
    cv::Mat image_matf;
    image_mat.convertTo(image_matf, CV_32FC3, 1 / 255.);
    if (skip_cvt_color) {
        memcpy(image_matf_color_data, image_matf.data, height * width * 3 * sizeof(float));
    } else {
        cv::Mat image_matf_color(height, width, CV_32FC3, image_matf_color_data);
        cvtColor(image_matf, image_matf_color, code);
    }
    return image_matf_color_data;
}

void scale_image(float *image, int height, int width, float *min_vals, float *max_vals) {
    // Check if we need to scale, some color spaces are already normalized
    int need_scale = 0;
    for (int j = 0; j < 3; ++j) {
        if (min_vals[j] != 0.)
            ++need_scale;
        if (max_vals[j] != 1.)
            ++need_scale;
    }
    if (!need_scale)
        return;
    int size = height * width * 3;
    float scale_vals[3];
    for (int j = 0; j < 3; ++j)
        scale_vals[j] = 1. / (max_vals[j] - min_vals[j]);
    for (int i = 0; i < size; i += 3) {
        for (int j = 0; j < 3; ++j)
            image[i + j] = (image[i + j] - min_vals[j]) * scale_vals[j];
    }
}
