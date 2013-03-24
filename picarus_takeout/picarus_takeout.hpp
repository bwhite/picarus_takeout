#ifndef PICARUS_TAKEOUT
#define PICARUS_TAKEOUT
#include "ImagePreprocessor.hpp"
#include "HistogramImageFeature.hpp"
#include "LinearClassifier.hpp"
#include "BlocksImageMultiFeature.hpp"
#include "LocalNBNNClassifier.hpp"
#include <string>
#include <vector>
#include <map>
#include <msgpack.hpp>
namespace Picarus {
ImagePreprocessor* image_preprocessor_factory(std::map<std::string, msgpack::object> *kw);
HistogramImageFeature* histogram_image_feature_factory(std::map<std::string, msgpack::object> *kw);
LinearClassifier* linear_classifier_factory(std::map<std::string, msgpack::object> *kw);
LocalNBNNClassifier* local_nbnn_classifier_factory(std::map<std::string, msgpack::object> *kw);
BlocksImageMultiFeature* blocks_image_multi_feature_factory(std::map<std::string, msgpack::object> *kw);
void delete_array(void *array);
} // namespace Picarus

#endif
