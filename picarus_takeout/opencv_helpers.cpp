#include "opencv_helpers.hpp"

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
