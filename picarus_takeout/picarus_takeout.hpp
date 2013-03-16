#ifndef PICARUS_TAKEOUT
#define PICARUS_TAKEOUT
#include "cJSON.h"
#include "ImagePreprocessor.hpp"
#include "HistogramImageFeature.hpp"
#include "LinearClassifier.hpp"

ImagePreprocessor* picarus_image_preprocessor_factory(cJSON *cjs);
HistogramImageFeature* picarus_histogram_image_feature_factory(cJSON *cjs);
LinearClassifier* picarus_linear_classifier_factory(cJSON *cjs);

#endif
