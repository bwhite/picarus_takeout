#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cassert>
#include "picarus_takeout.hpp"

namespace Picarus {

enum takeout_types {RAW_IMAGE, PROCESSED_IMAGE, FEATURE, BINARY_CLASS_CONFIDENCE, HASH, MULTI_FEATURE, MULTI_CLASS_DISTANCE};
void delete_array(void *array) {
    delete [] array;
}

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

ImagePreprocessor* image_preprocessor_factory(cJSON *cjs) {
    cJSON *kw = cJSON_GetObjectItem(cjs, "kw");
    cJSON *val;

    val = cJSON_GetObjectItem(kw, "method");
    if (!val || val->type != cJSON_String)
        return NULL;
    std::string method(val->valuestring);
    
    val = cJSON_GetObjectItem(kw, "compression");
    if (!val || val->type != cJSON_String)
        return NULL;
    std::string compression(val->valuestring);

    val = cJSON_GetObjectItem(kw, "size");
    if (!val || val->type != cJSON_Number)
        return NULL;
    int size = val->valueint;
    return new ImagePreprocessor(method, size, compression);
}


HistogramImageFeature* histogram_image_feature_factory(cJSON *cjs) {
    cJSON *kw = cJSON_GetObjectItem(cjs, "kw");
    cJSON *val;

    val = cJSON_GetObjectItem(kw, "mode");
    if (!val || val->type != cJSON_String)
        return NULL;
    std::string mode(val->valuestring);
    
    val = cJSON_GetObjectItem(kw, "levels");
    if (!val || val->type != cJSON_Number)
        return NULL;
    int levels = val->valueint;

    val = cJSON_GetObjectItem(kw, "num_bins"); // 3 ints
    std::vector<int> num_bins(3);
    if (!val)
        return NULL;
    if (val->type == cJSON_Number) {
        for (int i = 0; i < 3; ++i)
            num_bins[i] = val->valueint;
    } else {   
        if (val->type != cJSON_Array || cJSON_GetArraySize(val) != 3)
            return NULL;
        for (int i = 0; i < 3; ++i) {
            cJSON *array_val = cJSON_GetArrayItem(val, i);
            if (!array_val || array_val->type != cJSON_Number)
                return NULL;
            num_bins[i] = array_val->valueint;
        }
    }
    return new HistogramImageFeature(mode, num_bins, levels);
}

LinearClassifier* linear_classifier_factory(cJSON *cjs) {
    cJSON *kw = cJSON_GetObjectItem(cjs, "kw");
    cJSON *val;
    
    val = cJSON_GetObjectItem(kw, "intercept");
    if (!val || val->type != cJSON_Number)
        return NULL;
    double intercept = val->valuedouble;

    val = cJSON_GetObjectItem(kw, "coefficients");
    if (!val || val->type != cJSON_Array)
        return NULL;
    int num_coefficients = cJSON_GetArraySize(val);
    std::vector<double> coefficients(num_coefficients);
    for (int i = 0; i < num_coefficients; ++i) {
        cJSON *array_val = cJSON_GetArrayItem(val, i);
        if (!array_val || array_val->type != cJSON_Number)
            return NULL;
        coefficients[i] = array_val->valuedouble;
    }
    return new LinearClassifier(coefficients, intercept);
}


// chain = [{'model': model, 'input': input_type, 'output': output_type}]
void model_chain_factory(cJSON *cjs, unsigned char *input, int size) {
    return; // Obsolete
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
            ImagePreprocessor* ip = image_preprocessor_factory(model);
            assert(input_type == RAW_IMAGE);
            // TODO Fix
            //processed_image = ip->asbinary(input, size, &processed_image_height, &processed_image_width, &processed_image_channels);
            printf("height:%d width:%d\n", processed_image_height, processed_image_width);
            input_type = PROCESSED_IMAGE;
            delete ip;
        } else if (!strcmp("picarus.HistogramImageFeature", name->valuestring)) {
            HistogramImageFeature* hif = histogram_image_feature_factory(model);
            assert(input_type == PROCESSED_IMAGE);
            feature = hif->compute_feature(processed_image, processed_image_height, processed_image_width, &feature_size);
            for (int xx = 0; xx < feature_size; ++xx)
                printf("%f ", feature[xx]);
            printf("\n");
            delete [] processed_image;
            input_type = FEATURE;
            delete hif;
        } else if (!strcmp("picarus.LinearClassifier", name->valuestring)) {
            LinearClassifier* lc = linear_classifier_factory(model);
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
} // namespace Picarus
