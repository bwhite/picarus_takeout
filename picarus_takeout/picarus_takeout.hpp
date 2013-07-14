#ifndef PICARUS_TAKEOUT
#define PICARUS_TAKEOUT
#include "ImagePreprocessor.hpp"
#include "ImageWarp.hpp"
#include "HistogramImageFeature.hpp"
#include "GISTImageFeature.hpp"
#include "PixelsImageFeature.hpp"
#include "BOVWImageFeature.hpp"
#include "HOGImageMaskFeature.hpp"
#include "LinearClassifier.hpp"
#include "KernelClassifier.hpp"
#include "BlocksImageMultiFeature.hpp"
#include "LocalNBNNClassifier.hpp"
#include "FaceImageObjectDetector.hpp"
#include "BinaryPredictor.hpp"
#include "SphericalHasher.hpp"
#include "SphericalHashIndex.hpp"
#include "BRISKImageFeature2d.hpp"
#include "HammingFeature2dHashIndex.hpp"
#include <string>
#include <vector>
#include <map>
#include <msgpack.hpp>
namespace Picarus {
ImagePreprocessor* image_preprocessor_factory(std::map<std::string, msgpack::object> *kw);
ImageWarp* image_warp_factory(std::map<std::string, msgpack::object> *kw);
PixelsImageFeature* pixels_image_feature_factory(std::map<std::string, msgpack::object> *kw);
HistogramImageFeature* histogram_image_feature_factory(std::map<std::string, msgpack::object> *kw);
GISTImageFeature* gist_image_feature_factory(std::map<std::string, msgpack::object> *kw);
LinearClassifier* linear_classifier_factory(std::map<std::string, msgpack::object> *kw);
KernelClassifier* kernel_classifier_factory(std::map<std::string, msgpack::object> *kw);
LocalNBNNClassifier* local_nbnn_classifier_factory(std::map<std::string, msgpack::object> *kw);
BlocksImageMultiFeature* blocks_image_multi_feature_factory(std::map<std::string, msgpack::object> *kw);
HOGImageMaskFeature* hog_image_mask_feature_factory(std::map<std::string, msgpack::object> *kw);
BOVWImageFeature* bovw_image_feature_factory(std::map<std::string, msgpack::object> *kw);
FaceImageObjectDetector* face_image_object_detector_factory(std::map<std::string, msgpack::object> *kw);
BinaryPredictor* binary_predictor_factory(std::map<std::string, msgpack::object> *kw);
SphericalHasher* spherical_hasher_factory(std::map<std::string, msgpack::object> *kw);
SphericalHashIndex* spherical_hash_index_factory(std::map<std::string, msgpack::object> *kw);
BRISKImageFeature2d* brisk_image_feature2d_factory(std::map<std::string, msgpack::object> *kw);
HammingFeature2dHashIndex* hamming_feature2d_hash_index_factory(std::map<std::string, msgpack::object> *kw);
void delete_array(void *array);
} // namespace Picarus

#endif
