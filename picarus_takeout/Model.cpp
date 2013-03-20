#include <msgpack.hpp>
#include <cstring>
#include <opencv2/opencv.hpp> // TODO: Needed for image_bgr_fromstring, move to a "core" lib later
#include "Model.hpp"
#include <iostream>

namespace Picarus {

Model::Model() {}
Model::~Model() {}

unsigned char *Model::process_binary(const unsigned char *input, int size, int *size_out) {
    typedef struct {
        int size;
        unsigned char *data;
    } copy_collector_output_t;
    copy_collector_output_t output;
    process_binary(input, size, copy_collector, &output);
    *size_out = output.size;
    return output.data;
}


void ndarray_tostring(const std::vector<double> &vec, const std::vector<int> &shape, void (*collector)(const unsigned char *, int, void *), void *collector_state) {
    msgpack::type::tuple<std::vector<double>, std::vector<int> > tuple(vec, shape);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, tuple);
    collector((const unsigned char *)sbuf.data(), sbuf.size(), collector_state);
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

void double_tostring(double val, void (*collector)(const unsigned char *, int, void *), void *collector_state) {
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val);
    collector((const unsigned char *)sbuf.data(), sbuf.size(), collector_state);
}

void copy_collector(const unsigned char *input, int size, void *state) {
    typedef struct {
        int size;
        unsigned char *data;
    } copy_collector_output_t;
    copy_collector_output_t *output = (copy_collector_output_t *)state;
    output->data = new unsigned char[size];
    output->size = size;
    memcpy(output->data, input, output->size);
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
