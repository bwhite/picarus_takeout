#include <msgpack.hpp>
#include <cstring>
#include <opencv2/opencv.hpp> // TODO: Needed for image_bgr_fromstring, move to a "core" lib later
#include "Model.hpp"
#include <iostream>

namespace Picarus {

Model::Model() {}
Model::~Model() {}

unsigned char *Model::process_binary(const unsigned char *input, int size, int *size_out) {
    unsigned char *output;
    CopyCollector collector(&output, size_out);
    process_binary(input, size, &collector);
    return output;
}


void ndarray_tostring(const std::vector<double> &vec, const std::vector<int> &shape, BinaryCollector *collector) {
    msgpack::type::tuple<std::vector<double>, std::vector<int> > tuple(vec, shape);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, tuple);
    (*collector)((const unsigned char *)sbuf.data(), sbuf.size());
}

void ndarray_fromstring(const unsigned char *input, int size, std::vector<double> *vec, std::vector<int> *shape) {
    msgpack::unpacked msg;
    msgpack::sbuffer sbuf;
    sbuf.write((const char *)input, size);
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());
    msgpack::type::tuple<std::vector<double>, std::vector<int> > rvec(msg.get());
    msgpack::type::tuple_element<msgpack::type::tuple<std::vector<double>, std::vector<int> >, 0> rvec0(rvec);
    *vec = rvec0.get();
    msgpack::type::tuple_element<msgpack::type::tuple<std::vector<double>, std::vector<int> >, 1> rvec1(rvec);
    *shape = rvec1.get();
}

void double_fromstring(const unsigned char *input, int size, double *val) {
    msgpack::unpacked msg;
    msgpack::sbuffer sbuf;
    sbuf.write((const char *)input, size);
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());
    msg.get().convert(val);
}

void double_tostring(double val, BinaryCollector *collector) {
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val);
    (*collector)((const unsigned char *)sbuf.data(), sbuf.size());
}

unsigned char *image_bgr_fromstring(const unsigned char *binary_image, int size, int *height_out, int *width_out) {
    cv::Mat binary_image_mat(1, size, CV_8UC1, (unsigned char *)binary_image);
    cv::Mat image = cv::imdecode(binary_image_mat, CV_LOAD_IMAGE_COLOR);
    *height_out = image.rows;
    *width_out = image.cols;
    if (image.channels() != 3) {
        *height_out = 0;
        *width_out = 0;
        return NULL;
    }
    unsigned char *p = new unsigned char[image.rows * image.cols * image.channels()];
    memcpy(p, image.ptr<unsigned char>(0), image.rows * image.cols * image.channels()); // TODO: Remove Copy!
    return p;
}
} // namespace Picarus
