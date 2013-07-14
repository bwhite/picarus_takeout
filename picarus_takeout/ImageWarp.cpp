#include "ImageWarp.hpp"
#include "opencv_helpers.hpp"
#include <cstring>
namespace Picarus {

// TODO: this shares code with image preprocessor, should refactor

ImageWarp::ImageWarp(std::vector<double> h, int height, int width, std::string compression) : height(height), width(width) {
    this->h = h;
    this->valid = h.size() == 9;
    if (compression.compare("png") == 0) {
        this->compression_extension = std::string(".png");
        std::vector<int> params;
        this->compression_params = params;
    } else if (compression.compare("ppm") == 0) {
        this->compression_extension = std::string(".ppm");
        std::vector<int> params;
        this->compression_params = params;
    } else {
        this->compression_extension = std::string(".jpg");
        std::vector<int> params(2);
        params[0] = CV_IMWRITE_JPEG_QUALITY;
        params[1] = 100;
        this->compression_params = params;
    }
}


ImageWarp::~ImageWarp() {
}

unsigned char *ImageWarp::asbinary(const unsigned char *binary_image, int size, int *size_out) {
    if (!valid) {
        *size_out = 0;
        return 0;
    }
    std::vector<unsigned char> buf;
    {
        int height, width;
        unsigned char *image_data = asarray(binary_image, size, &height, &width);
        if (!height || !width) {
            *size_out = 0;
            return NULL;
            free(image_data);
        }
        cv::Mat image(height, width, CV_8UC3, image_data);
        cv::imencode(compression_extension, image, buf, compression_params);
        delete [] image_data;
    }
    unsigned char *out_data = new unsigned char[buf.size()];
    memcpy(out_data, &buf[0], buf.size());
    *size_out = buf.size();
    return out_data;
}

unsigned char *ImageWarp::asarray(const unsigned char *binary_image, int size, int *height_out, int *width_out) {
    if (!valid) {
        *height_out = *width_out = 0;
        return 0;
    }
    cv::Mat binary_image_mat(1, size, CV_8UC1, (unsigned char *)binary_image);
    int orig_height, orig_width;
    unsigned char *image_data = image_bgr_fromstring(binary_image, size, &orig_height, &orig_width);
    cv::Mat image(orig_height, orig_width, CV_8UC3, image_data);

    unsigned char *p = new unsigned char[this->height * this->width * 3];

    // NOTE: This assumed we are only using BGR output, may change in the future
    cv::Mat image_resized(this->height, this->width, CV_8UC3, p);
    cv::Mat m(3, 3, CV_64FC1, &this->h[0]);
    cv::warpPerspective(image, image_resized, m, cv::Size(this->width, this->height));
    *height_out = this->height;
    *width_out = this->width;
    delete [] image_data;
    return p;
}

void ImageWarp::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    int size_out;
    unsigned char *output = asbinary(input, size, &size_out);
    (*collector)(output, size_out);
    delete [] output;
}
} // namespace Picarus
