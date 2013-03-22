#ifndef PICARUS_MODEL
#define PICARUS_MODEL
#include <string>
#include <vector>
#include <cstdio>

namespace Picarus {
class Model {
public:
    Model();
    virtual ~Model();
    virtual void process_binary(const unsigned char *input, int size, void (*collector)(const unsigned char *, int, void *), void *collector_state) = 0;
    virtual unsigned char *process_binary(const unsigned char *input, int size, int *size_out);
};
void ndarray_fromstring(const unsigned char *input, int size, std::vector<double> *vec, std::vector<int> *shape);
void ndarray_tostring(const std::vector<double> &vec, const std::vector<int> &shape, void (*collector)(const unsigned char *, int, void *), void *collector_state);
void double_fromstring(const unsigned char *input, int size, double *val);
void double_tostring(double val, void (*collector)(const unsigned char *, int, void *), void *collector_state);
void copy_collector(const unsigned char *input, int size, void *state);
unsigned char *image_bgr_fromstring(const unsigned char *binary_image, int size, int *height_out, int *width_out);
} // namespace Picarus

#endif
