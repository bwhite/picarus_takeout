#ifndef IMAGE_FEATURE
#define IMAGE_FEATURE
#include <vector>
#include "Model.hpp"

namespace Picarus {
class ImageFeature : public Model {
public:
    ImageFeature();
    virtual ~ImageFeature();
    virtual double* compute_feature(unsigned char *image, int height, int width, int *out_size) = 0;
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};
} // namespace Picarus
#endif
