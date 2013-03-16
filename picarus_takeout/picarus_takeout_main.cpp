#include <fstream>
#include <streambuf>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
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

    cJSON_Delete(cjs);
    return 0;
}
