#include "opencv_helpers.hpp"
#include "BlocksImageMultiFeature.hpp"
#include <cstdio>

namespace Picarus {

BlocksImageMultiFeature::BlocksImageMultiFeature(int block_size, std::string mode, int levels) : block_size(block_size), mode(mode), levels(levels) {
    color_mode_to_code(mode, min_vals, max_vals, &skip_cvt_color, &code);
}

BlocksImageMultiFeature::~BlocksImageMultiFeature() {
}


double *BlocksImageMultiFeature::compute_multi_feature(unsigned char *image, int height, int width, int *num_features_out, int *feature_size_out) {
    int cur_height = height, cur_width = width, prev_height = height, prev_width = width, total_blocks = 0;
    // Compute # of blocks so that we can preallocate the feature array
    for (int i = 0; i < levels; ++i) {
        total_blocks += (cur_height / block_size) * (cur_width / block_size);
        cur_height /= 2;
        cur_width /= 2;
    }
    double *features = new double[block_size * block_size * total_blocks * 3];
    double *cur_feature = features;
    unsigned char *prev_image = image, *cur_image = image;
    prev_height = cur_height = height;
    prev_width = cur_width = width;
    for (int i = 0; i < levels; ++i) {
        if (i != 0) {
            cur_image = new unsigned char[cur_height * cur_width * 3];
            cv::Mat prev_image_mat(prev_height, prev_width, CV_8UC3, prev_image);
            cv::Mat cur_image_mat(cur_height, cur_width, CV_8UC3, cur_image);
            cv::resize(prev_image_mat, cur_image_mat, cv::Size(cur_width, cur_height));
        }
        int height_offset = cur_height % block_size / 2;
        int width_offset = cur_width % block_size / 2;
        printf("prev[%d,%d] cur[%d,%d] Offset[%d,%d]\n", prev_height, prev_width, cur_height, cur_width, height_offset, width_offset);
        float *cur_image_color = convert_color(cur_image, cur_height, cur_width, code, skip_cvt_color);
        scale_image(cur_image_color, cur_height, cur_width, min_vals, max_vals);
        // Add features to list:  Take a deep breath before moving on...
        for (int j = 0; j < cur_height / block_size; ++j)
            for (int k = 0; k < cur_width / block_size; ++k) {
                // cur_feature: Holds block_size * block_size * 3 values
                for (int l = 0; l < block_size; ++l)
                    for (int m = 0; m < block_size; ++m)
                        for (int n = 0; n < 3; ++n)
                            cur_feature[(l * block_size + m) * 3 + n] = cur_image_color[((j * block_size + l + height_offset) * cur_width + k * block_size + m + width_offset) * 3 + n];
                cur_feature += block_size * block_size * 3;
            }
        // Cleanup for next level
        delete [] cur_image_color;
        if (prev_image != image)
            delete [] prev_image;
        prev_image = cur_image;
        prev_height = cur_height;
        prev_width = cur_width;
        cur_height /= 2;
        cur_width /= 2;
    }
    if (prev_image != image)
        delete [] prev_image;
    printf("BlocksImage: num_features[%d]\n", total_blocks);
    *num_features_out = total_blocks;
    *feature_size_out = block_size * block_size * 3;
    return features;
}
} // namespace Picarus
