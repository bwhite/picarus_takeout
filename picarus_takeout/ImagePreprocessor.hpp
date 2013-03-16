#ifndef IMAGE_PREPROCESSOR
#define IMAGE_PREPROCESSOR
#include <string>
#include <vector>

class ImagePreprocessor {
private:
    const std::string method, compression;
    const int size;
    int method_code;
public:
    ImagePreprocessor(std::string method, int size, std::string compression);
    ~ImagePreprocessor();
    std::vector<char> asbinary(std::vector<char> binary_image);

    /* NOTE: Receiver must call picarus_delete_array(image) on the returned value */
    unsigned char *asarray(unsigned char *binary_image, int size, int *height_out, int *width_out, int *channels_out);
};

#endif
