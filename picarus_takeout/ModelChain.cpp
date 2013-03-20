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
    const unsigned char *cur_buf = NULL;
    const unsigned char *next_buf = NULL;
    for (int i = 0; i < models.size(); ++i) {
        models[i]->process_binary(input, size, copy_collector, &next_buf);
        if (cur_buf != NULL)
            delete [] cur_buf;
        cur_buf = next_buf;
        next_buf = NULL;
        size = *((int *)cur_buf);
        input = cur_buf + sizeof(int);
    }
    collector(input, size, collector_state);
    if (cur_buf != NULL)
        delete [] cur_buf;
}

} // namespace Picarus
