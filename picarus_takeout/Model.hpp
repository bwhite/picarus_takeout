#ifndef PICARUS_MODEL
#define PICARUS_MODEL
#include <string>
#include <vector>
#include <cstdio>
#include <utility>
#include <map>
#include "BinaryCollector.hpp"

namespace Picarus {
class Model {
public:
    Model();
    virtual ~Model();
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector) = 0;
    virtual unsigned char *process_binary(const unsigned char *input, int size, int *size_out);
};
void string_pair_fromstring(const unsigned char *input, int size, std::pair<std::string, std::string> *val);
void string_pair_tostring(const std::pair<std::string, std::string> &val,  BinaryCollector *collector);
void ndarray_fromstring(const unsigned char *input, int size, std::vector<double> *vec, std::vector<int> *shape);
void ndarray_tostring(const std::vector<double> &vec, const std::vector<int> &shape, BinaryCollector *collector);
void double_fromstring(const unsigned char *input, int size, double *val);
void double_tostring(double val, BinaryCollector *collector);
void bool_fromstring(const unsigned char *input, int size, bool *val);
void bool_tostring(bool val, BinaryCollector *collector);
void double_strings_fromstring(const unsigned char *input, int size, std::vector<std::pair<double, std::string> > *val);
void double_strings_tostring(const std::vector<std::pair<double, std::string> > &double_strings,  BinaryCollector *collector);
void image_detections_tostring(const std::string &image_str, const std::vector<double> &vec, const std::vector<int> &shape, BinaryCollector *collector);
void image_detections_fromstring(const unsigned char *input, int size, std::string *image_str, std::vector<double> *vec, std::vector<int> *shape);
unsigned char *image_bgr_fromstring(const unsigned char *binary_image, int size, int *height_out, int *width_out);
void labels_dists_tostring(const std::vector<std::string> &labels, const std::vector<double> &dists, BinaryCollector *collector);
void labels_dists_fromstring(const unsigned char *input, int size, std::vector<std::string> *labels, std::vector<double> *dists);
void binary_feature2d_tostring(const std::string &features, const std::vector<double> &keypoints, const std::vector<int> &shape, BinaryCollector *collector);
void binary_feature2d_fromstring(const unsigned char *input, int size, std::string *features, std::vector<double> *vec, std::vector<int> *shape);
} // namespace Picarus

#endif
