#include "ModelChain.hpp"
#include "picarus_takeout.hpp"
#include <cstring>

namespace Picarus {

ModelChain::ModelChain(const char *json_config) {
    cJSON *cjs = cJSON_Parse(json_config);
    if (cjs->type != cJSON_Array)
        return;
    int num_links = cJSON_GetArraySize(cjs);
    models.resize(num_links);
    for (int i = 0; i < num_links; ++i) {
        cJSON *link = cJSON_GetArrayItem(cjs, i);
        cJSON *model = cJSON_GetObjectItem(link, "model");
        cJSON *name = cJSON_GetObjectItem(model, "name");
        if (!name || name->type != cJSON_String|| !name->valuestring)
            return;
        if (!strcmp("picarus.ImagePreprocessor", name->valuestring))
            models[i] = image_preprocessor_factory(model);
        else if (!strcmp("picarus.HistogramImageFeature", name->valuestring))
            models[i] = histogram_image_feature_factory(model);
        else if (!strcmp("picarus.LinearClassifier", name->valuestring))
            models[i] = linear_classifier_factory(model);
        else
            return;
    }
    cJSON_Delete(cjs);
}


ModelChain::~ModelChain() {
    for (int i = 0; i < models.size(); ++i) {
        delete models[i];
    }
}

void ModelChain::process_binary(const unsigned char *input, int size, void (*collector)(const unsigned char *, int, void *), void *collector_state) {
    typedef struct {
        int size;
        const unsigned char *data;
    } copy_collector_output_t;
    copy_collector_output_t cur_buf = {size, input};
    copy_collector_output_t next_buf = {};
    for (int i = 0; i < models.size(); ++i) {
        models[i]->process_binary(cur_buf.data, cur_buf.size, copy_collector, &next_buf);
        if (cur_buf.data != input)
            delete [] cur_buf.data;
        cur_buf = next_buf;
    }
    collector(cur_buf.data, cur_buf.size, collector_state);
    if (cur_buf.data != input)
        delete [] cur_buf.data;
}

} // namespace Picarus
