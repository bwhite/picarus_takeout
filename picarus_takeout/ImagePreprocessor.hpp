#ifndef IMAGE_PREPROCESSOR
#define IMAGE_PREPROCESSOR
#include <string>
#include <vector>
#include "Model.hpp"

namespace Picarus {
class ImagePreprocessor : public Model {
private:
    const std::string method;
    std::string compression_extension;
    std::vector<int> compression_params;
    const int size;
    int method_code;
protected:
    unsigned char *asarray(const unsigned char *binary_image, int size, int *height_out, int *width_out);
public:
    ImagePreprocessor(std::string method, int size, std::string compression);
    virtual ~ImagePreprocessor();
    /* NOTE: Receiver must call picarus_delete_array(image) on the returned value */
    unsigned char *asbinary(const unsigned char *binary_image, int size, int *size_out);
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};
} // namespace Picarus
#endif
