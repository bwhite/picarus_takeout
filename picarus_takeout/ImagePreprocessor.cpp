#include "ImagePreprocessor.hpp"
#include <opencv2/opencv.hpp>
#include <cstring>
#include <cstdio>

ImagePreprocessor::ImagePreprocessor(std::string method, int size, std::string compression) : method(method), size(size), compression(compression) {
    if (!method.compare("max_side")) {
        this->method_code = 0;
    } else if (!method.compare("force_max_side")) {
        this->method_code = 1;
    } else if (!method.compare("force_square")) {
        this->method_code = 2;
    } else {
        this->method_code = 255; // "original"  Don't change the size
    }
}


ImagePreprocessor::~ImagePreprocessor() {
}

std::vector<char> ImagePreprocessor::asbinary(std::vector<char> binary_image) {
    int height, width, channels;
    //std::vector<unsigned char> asarray(binary_image, &height, &width, &channels);
    //cv::imencode();
    //bool imencode(const string& ext, InputArray img, vector<uchar>& buf, const vector<int>& params=vector<int>())
    // TODO: Convert
}

unsigned char *ImagePreprocessor::asarray(unsigned char *binary_image, int size, int *height_out, int *width_out, int *channels_out) {
    cv::Mat binary_image_mat(1, size, CV_8UC1, binary_image);
    cv::Mat image = cv::imdecode(binary_image_mat, CV_LOAD_IMAGE_COLOR);
    unsigned char *image_cropped_data = NULL;
    int orig_height = image.rows, orig_width = image.cols;
    int new_height = orig_height, new_width = orig_width;
    switch (this->method_code) {
    case 0: // max_side
    {
        if (orig_height <= this->size && orig_width <= this->size)
            break;
    }
    case 1: // force_max_side
    {
        if (orig_height >= orig_width) {
            new_height = this->size;
            new_width = this->size * (double)orig_width / orig_height + .5;
        } else {
            new_width = this->size;
            new_height = this->size * (double)orig_height / orig_width + .5;
        }
        break;
    }
    case 2: // force_square
    {
        if (orig_width != orig_height) {
            int new_size = orig_width > orig_height ? orig_height : orig_width;
            image_cropped_data = new unsigned char[new_size * new_size * 3];
            cv::Mat image_cropped(new_size, new_size, CV_8UC3, image_cropped_data);
            cv::getRectSubPix(image, cv::Size2f(new_size, new_size),  cv::Point2f(orig_width / 2., orig_width / 2.), image_cropped);
            image = image_cropped;
            orig_width = orig_height = new_size;
        }
        new_height = new_width = this->size;
        break;
    }
    default:
        break;
    }

    unsigned char *p = new unsigned char[new_height * new_width * 3];
    if (orig_width != new_width || orig_height != new_height) {
        // NOTE: This assumed we are only using BGR output, may change in the future
        cv::Mat image_resized(new_height, new_width, CV_8UC3, p);
        int interpolation = CV_INTER_LINEAR;
        if ((double)new_width / orig_width < .5 || (double)new_height / orig_height < .5)
            interpolation = CV_INTER_AREA;
        cv::resize(image, image_resized, cv::Size(new_width, new_height), interpolation);
    } else {
        memcpy(p, image.ptr<unsigned char>(0), image.rows * image.cols * image.channels());
    }
    *height_out = new_height;
    *width_out = new_width;
    *channels_out = image.channels();
    if (image_cropped_data != NULL)
        delete [] image_cropped_data;
    return p;
}
