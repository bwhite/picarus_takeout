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
void ndarray_fromstring(const unsigned char *input, int size, std::vector<double> *vec, std::vector<int> *shape);
void ndarray_tostring(const std::vector<double> &vec, const std::vector<int> &shape, BinaryCollector *collector);
void double_fromstring(const unsigned char *input, int size, double *val);
void double_tostring(double val, BinaryCollector *collector);
void double_strings_fromstring(const unsigned char *input, int size, std::vector<std::pair<double, std::string> > *val);
void double_strings_tostring(const std::vector<std::pair<double, std::string> > &double_strings,  BinaryCollector *collector);
unsigned char *image_bgr_fromstring(const unsigned char *binary_image, int size, int *height_out, int *width_out);
} // namespace Picarus

#endif
