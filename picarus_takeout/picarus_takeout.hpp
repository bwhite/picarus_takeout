#ifndef PICARUS_TAKEOUT
#define PICARUS_TAKEOUT
#include "cJSON.h"
#include "ImagePreprocessor.hpp"
#include "HistogramImageFeature.hpp"
#include "LinearClassifier.hpp"
namespace Picarus {
ImagePreprocessor* image_preprocessor_factory(cJSON *cjs);
HistogramImageFeature* histogram_image_feature_factory(cJSON *cjs);
LinearClassifier* linear_classifier_factory(cJSON *cjs);
void model_chain_factory(cJSON *cjs, unsigned char *input, int size);
void delete_array(void *array);
} // namespace Picarus

#endif
