#include <vector>
#include <string>
#include "picarus_takeout.hpp"


ImagePreprocessor* picarus_image_preprocessor_factory(cJSON *cjs) {
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


HistogramImageFeature* picarus_histogram_image_feature_factory(cJSON *cjs) {
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
    if (!val || val->type != cJSON_Array || cJSON_GetArraySize(val) != 3)
        return NULL;
    std::vector<int> num_bins(3);
    for (int i = 0; i < 3; ++i) {
        cJSON *array_val = cJSON_GetArrayItem(val, i);
        if (!array_val || array_val->type != cJSON_Number)
            return NULL;
        num_bins[i] = array_val->valueint;
    }
    return new HistogramImageFeature(mode, num_bins, levels);
}

LinearClassifier* picarus_linear_classifier_factory(cJSON *cjs) {
    cJSON *kw = cJSON_GetObjectItem(cjs, "kw");
    cJSON *val;
    
    val = cJSON_GetObjectItem(kw, "intercept");
    if (!val || val->type != cJSON_Number)
        return NULL;
    int intercept = val->valuedouble;

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
