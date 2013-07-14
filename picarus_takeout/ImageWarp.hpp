#ifndef IMAGE_WARP
#define IMAGE_WARP
#include <string>
#include <vector>
#include "Model.hpp"

namespace Picarus {
class ImageWarp : public Model {
private:
    std::vector<double> h;
    const int height;
    const int width;
    int valid;
    std::string compression_extension;
    std::vector<int> compression_params;
protected:
    unsigned char *asarray(const unsigned char *binary_image, int size, int *height_out, int *width_out);
public:
    ImageWarp(std::vector<double> h, int height, int width, std::string compression);
    virtual ~ImageWarp();
    /* NOTE: Receiver must call picarus_delete_array(image) on the returned value */
    unsigned char *asbinary(const unsigned char *binary_image, int size, int *size_out);
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};
} // namespace Picarus
#endif
