#ifndef IMAGE_PREPROCESSOR
#define IMAGE_PREPROCESSOR
#include <string>
#include <vector>

class ImagePreprocessor {
private:
    const std::string method;
    std::string compression_extension;
    std::vector<int> compression_params;
    const int size;
    int method_code;
public:
    ImagePreprocessor(std::string method, int size, std::string compression);
    ~ImagePreprocessor();
    /* NOTE: Receiver must call picarus_delete_array(image) on the returned value */
    unsigned char *asbinary(unsigned char *binary_image, int size, int *size_out);
    unsigned char *asarray(unsigned char *binary_image, int size, int *height_out, int *width_out, int *channels_out);
};

#endif
