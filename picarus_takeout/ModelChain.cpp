#include "ModelChain.hpp"
#include "picarus_takeout.hpp"
#include <cstring>

namespace Picarus {
ModelChain::ModelChain(const char *msgpack_binary, int size) {
    msgpack::unpacked msg;
    msgpack::unpack(&msg, msgpack_binary, size);
    msgpack::object obj = msg.get();
    std::vector<std::map<std::string, msgpack::object> > models;
    obj >> models;
    std::vector<std::map<std::string, msgpack::object> >::iterator model;
    this->models.reserve(models.size());
    for (model = models.begin(); model != models.end(); ++model) {
        std::string name;
        std::map<std::string, msgpack::object> kw;
        model->at(std::string("name")) >> name;
        model->at(std::string("kw")) >> kw;
        if (!name.compare("picarus.ImagePreprocessor"))
            this->models.push_back(image_preprocessor_factory(&kw));
        else if (!name.compare("picarus.HistogramImageFeature"))
            this->models.push_back(histogram_image_feature_factory(&kw));
        else if (!name.compare("picarus.LinearClassifier"))
            this->models.push_back(linear_classifier_factory(&kw));
        else {
            printf("Error: Unknown name[%s]\n", name.c_str());
            this->models.resize(0);
            return;
        }
        if (this->models.back() == NULL) {
            printf("Error: Could not create[%s]\n", name.c_str());
            this->models.resize(0);
            return;
        }
    }
}

ModelChain::~ModelChain() {
    for (int i = 0; i < models.size(); ++i) {
        delete models[i];
    }
}

void ModelChain::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    int cur_size = size, next_size;
    const unsigned char *cur_data = input;
    unsigned char *next_data;
    CopyCollector collector_inner(&next_data, &next_size);
    for (int i = 0; i < models.size(); ++i) {
        models[i]->process_binary(cur_data, cur_size, &collector_inner);
        if (cur_data != input)
            delete [] cur_data;
        cur_data = next_data;
        cur_size = next_size;
        if (next_data == NULL) {
            printf("ModelChain: Model[%d] returned NULL\n", i);
            (*collector)(NULL, 0);
            return;
        }
            
    }
    (*collector)(cur_data, cur_size);
    if (cur_data != input)
        delete [] cur_data;
}

} // namespace Picarus
