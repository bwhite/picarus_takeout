#include <fstream>
#include <streambuf>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cassert>
#include "picarus_takeout.hpp"

void read_file(const char *fn, std::string *str) {
    std::ifstream t(fn);
    t.seekg(0, std::ios::end);   
    str->reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str->assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
}

void read_file(const char *fn, std::vector<char> *str) {
    std::ifstream t(fn);
    t.seekg(0, std::ios::end);   
    str->reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str->assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
}

enum takeout_types {RAW_IMAGE, PROCESSED_IMAGE, FEATURE, BINARY_CLASS_CONFIDENCE, HASH, MULTI_FEATURE, MULTI_CLASS_DISTANCE};

static int type_to_code(const char *type) {
    if (!strcmp("raw_image", type))
        return RAW_IMAGE;
    else if (!strcmp("processed_image", type))
        return PROCESSED_IMAGE;
    else if (!strcmp("feature", type))
        return FEATURE;
    else if (!strcmp("binary_class_confidence", type))
        return BINARY_CLASS_CONFIDENCE;
    else if (!strcmp("hash", type))
        return HASH;
    else if (!strcmp("multi_feature", type))
        return MULTI_FEATURE;
    else if (!strcmp("multi_class_distance", type))
        return MULTI_CLASS_DISTANCE;
    return -1;
}

// chain = [{'model': model, 'input': input_type, 'output': output_type}]
void picarus_takeout_chain_factory(cJSON *cjs, unsigned char *input, int input_size) {
    printf("type:[%d]\n", cjs->type);
    if (cjs->type != cJSON_Array) {
        return;
    }
    int input_type = RAW_IMAGE;

    /* processed_image */
    int processed_image_width, processed_image_height, processed_image_channels;
    unsigned char *processed_image;

    /* feature */
    double *feature;
    int feature_size;

    /* binary_class_confidence */
    double confidence;

    int num_links = cJSON_GetArraySize(cjs);
    for (int i = 0; i < num_links; ++i) {
        cJSON *link = cJSON_GetArrayItem(cjs, i);
        cJSON *model_input = cJSON_GetObjectItem(link, "input");
        cJSON *model_output = cJSON_GetObjectItem(link, "output");
        cJSON *model = cJSON_GetObjectItem(link, "model");
        cJSON *name = cJSON_GetObjectItem(model, "name");
        printf("name:[%s]\n", name->valuestring);
        if (!name || name->type != cJSON_String|| !name->valuestring) {
            return;
        }

        if (!strcmp("picarus.ImagePreprocessor", name->valuestring)) {
            ImagePreprocessor* ip = picarus_image_preprocessor_factory(model);
            assert(input_type == RAW_IMAGE);
            processed_image = ip->asarray(input, input_size, &processed_image_height, &processed_image_width, &processed_image_channels);
            printf("height:%d width:%d\n", processed_image_height, processed_image_width);
            input_type = PROCESSED_IMAGE;
            delete ip;
        } else if (!strcmp("picarus.HistogramImageFeature", name->valuestring)) {
            HistogramImageFeature* hif = picarus_histogram_image_feature_factory(model);
            assert(input_type == PROCESSED_IMAGE);
            feature = hif->compute_feature(processed_image, processed_image_height, processed_image_width, &feature_size);
            for (int xx = 0; xx < feature_size; ++xx)
                printf("%f ", feature[xx]);
            printf("\n");
            delete [] processed_image;
            input_type = FEATURE;
            delete hif;
        } else if (!strcmp("picarus.LinearClassifier", name->valuestring)) {
            LinearClassifier* lc = picarus_linear_classifier_factory(model);
            assert(input_type == FEATURE);
            confidence = lc->decision_function(feature, feature_size);
            delete [] feature;
            input_type = BINARY_CLASS_CONFIDENCE;
            delete lc;
        } else {
            return;
        }
    }
    if (input_type == BINARY_CLASS_CONFIDENCE)
        printf("%f\n", confidence);
}


int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <config_json_path> <input_path>" << std::endl;
        return 1;
    }
    std::string json_config;
    read_file(argv[1], &json_config);
    std::vector<char> input_data;
    read_file(argv[2], &input_data);
    cJSON *cjs = cJSON_Parse(json_config.c_str());
    printf("type:[%d]\n", cjs->type);
    picarus_takeout_chain_factory(cjs, (unsigned char *)&input_data[0], input_data.size());
/*
    cJSON *cjs, unsigned char *input, int input_size, int *size_out

    cJSON *name = cJSON_GetObjectItem(cjs, "name");

    if (name && strcmp("picarus.ImagePreprocessor", name->valuestring) == 0) {
        ImagePreprocessor* ip = picarus_image_preprocessor_factory(cjs);
        int width, height, channels;
        unsigned char *image = ip->asarray((unsigned char *)&input_data[0], input_data.size(), &height, &width, &channels);
        std::cout << "Height: " << height << " Width: " << width << " Channels: " << channels << std::endl;
        free(image);
        delete ip;
    } else if (name && strcmp("picarus.HistogramImageFeature", name->valuestring) == 0) {
        HistogramImageFeature* hif = picarus_histogram_image_feature_factory(cjs);
        delete hif;
    } else if (name && strcmp("picarus.LinearClassifier", name->valuestring) == 0) {
        double val[] = {3., 4.};
        LinearClassifier* lc = picarus_linear_classifier_factory(cjs);
        std::cout << "Val:" << lc->decision_function(val, 2) << std::endl;
        delete lc;
    }
*/
    cJSON_Delete(cjs);
    return 0;
}
