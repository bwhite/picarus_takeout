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

void double_strings_fromstring(const unsigned char *input, int size, std::vector<std::pair<double, std::string> > *val) {
    msgpack::unpacked msg;
    msgpack::unpack(&msg, (const char *)input, size);
    msgpack::object obj = msg.get();
    obj >> *val;
}

void double_strings_tostring(const std::vector<std::pair<double, std::string> > &double_strings,  BinaryCollector *collector) {
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, double_strings);
    (*collector)((const unsigned char *)sbuf.data(), sbuf.size());
}

void ndarray_tostring(const std::vector<double> &vec, const std::vector<int> &shape, BinaryCollector *collector) {
    msgpack::type::tuple<std::vector<double>, std::vector<int> > tuple(vec, shape);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, tuple);
    (*collector)((const unsigned char *)sbuf.data(), sbuf.size());
}

void binary_feature2d_tostring(const std::string &features, const std::vector<double> &keypoints, const std::vector<int> &shape, BinaryCollector *collector) {
    msgpack::type::tuple<std::string, std::vector<double>, std::vector<int> > tuple(features, keypoints, shape);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, tuple);
    (*collector)((const unsigned char *)sbuf.data(), sbuf.size());
}

void binary_feature2d_fromstring(const unsigned char *input, int size, std::string *features, std::vector<double> *vec, std::vector<int> *shape) {
    // TODO: Look at optimizing this
    msgpack::unpacked msg;
    msgpack::sbuffer sbuf;
    sbuf.write((const char *)input, size);
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());
    msgpack::type::tuple<std::string, std::vector<double>, std::vector<int> > rvec(msg.get());
    msgpack::type::tuple_element<msgpack::type::tuple<std::string, std::vector<double>, std::vector<int> >, 0> rvec0(rvec);
    *features = rvec0.get();
    msgpack::type::tuple_element<msgpack::type::tuple<std::string, std::vector<double>, std::vector<int> >, 1> rvec1(rvec);
    *vec = rvec1.get();
    msgpack::type::tuple_element<msgpack::type::tuple<std::string, std::vector<double>, std::vector<int> >, 2> rvec2(rvec);
    *shape = rvec2.get();
}



void image_detections_tostring(const std::string &image_str, const std::vector<double> &vec, const std::vector<int> &shape, BinaryCollector *collector) {
    msgpack::type::tuple<std::string, std::vector<double>, std::vector<int> > tuple(image_str, vec, shape);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, tuple);
    (*collector)((const unsigned char *)sbuf.data(), sbuf.size());
}

void image_detections_fromstring(const unsigned char *input, int size, std::string *image_str, std::vector<double> *vec, std::vector<int> *shape) {
    msgpack::unpacked msg;
    msgpack::sbuffer sbuf;
    sbuf.write((const char *)input, size);
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());
    msgpack::type::tuple<std::string, std::vector<double>, std::vector<int> > rvec(msg.get());
    msgpack::type::tuple_element<msgpack::type::tuple<std::string, std::vector<double>, std::vector<int> >, 0> rvec0(rvec);
    msgpack::type::tuple_element<msgpack::type::tuple<std::string, std::vector<double>, std::vector<int> >, 1> rvec1(rvec);
    msgpack::type::tuple_element<msgpack::type::tuple<std::string, std::vector<double>, std::vector<int> >, 2> rvec2(rvec);
    *image_str = rvec0.get();
    *vec = rvec1.get();
    *shape = rvec2.get();
}

void ndarray_fromstring(const unsigned char *input, int size, std::vector<double> *vec, std::vector<int> *shape) {
    // TODO: Look at optimizing this
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
    // TODO: Look at optimizing this
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

void bool_fromstring(const unsigned char *input, int size, bool *val) {
    // TODO: Look at optimizing this
    msgpack::unpacked msg;
    msgpack::sbuffer sbuf;
    sbuf.write((const char *)input, size);
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());
    msg.get().convert(val);
}

void bool_tostring(bool val, BinaryCollector *collector) {
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
