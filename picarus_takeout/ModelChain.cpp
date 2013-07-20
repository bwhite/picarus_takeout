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
        else if (!name.compare("picarus.ImageWarp"))
            this->models.push_back(image_warp_factory(&kw));
        else if (!name.compare("picarus.HistogramImageFeature"))
            this->models.push_back(histogram_image_feature_factory(&kw));
        else if (!name.compare("picarus.GISTImageFeature"))
            this->models.push_back(gist_image_feature_factory(&kw));
        else if (!name.compare("picarus.BOVWImageFeature"))
            this->models.push_back(bovw_image_feature_factory(&kw));
        else if (!name.compare("picarus.PixelsImageFeature"))
            this->models.push_back(pixels_image_feature_factory(&kw));
        else if (!name.compare("picarus.HOGImageMaskFeature"))
            this->models.push_back(hog_image_mask_feature_factory(&kw));
        else if (!name.compare("picarus.LinearClassifier"))
            this->models.push_back(linear_classifier_factory(&kw));
        else if (!name.compare("picarus.KernelClassifier"))
            this->models.push_back(kernel_classifier_factory(&kw));
        else if (!name.compare("picarus.BRISKImageFeature2d"))
            this->models.push_back(brisk_image_feature2d_factory(&kw));
        else if (!name.compare("picarus.FREAKImageFeature2d"))
            this->models.push_back(freak_image_feature2d_factory(&kw));
        else if (!name.compare("picarus.BinaryPredictor"))
            this->models.push_back(binary_predictor_factory(&kw));
        else if (!name.compare("picarus.SphericalHasher"))
            this->models.push_back(spherical_hasher_factory(&kw));
        else if (!name.compare("picarus.SphericalHashIndex"))
            this->models.push_back(spherical_hash_index_factory(&kw));
        else if (!name.compare("picarus.HammingFeature2dHashIndex"))
            this->models.push_back(hamming_feature2d_hash_index_factory(&kw));
        else if (!name.compare("picarus.BlocksImageMultiFeature"))
            this->models.push_back(blocks_image_multi_feature_factory(&kw));
        else if (!name.compare("picarus.LocalNBNNClassifier"))
            this->models.push_back(local_nbnn_classifier_factory(&kw));
        else if (!name.compare("picarus.FaceImageObjectDetector"))
            this->models.push_back(face_image_object_detector_factory(&kw));
        else if (!name.compare("picarus.ImageMatcherHammingRansac"))
            this->models.push_back(image_matcher_hamming_ransac_factory(&kw));
        else {
            printf("Error: Unknown name[%s]\n", name.c_str());
            this->models.resize(0);
            break;
        }
        if (this->models.back() == NULL) {
            printf("Error: Could not create[%s]\n", name.c_str());
            this->models.resize(0);
            break;
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
#ifdef DEBUG
        printf("Model Output[%d]:", i);
        for (int j = 0; j < cur_size; ++j)
            printf(" %d", (int)cur_data[j]);
        printf("\n");
#endif
    }
    (*collector)(cur_data, cur_size);
    if (cur_data != input)
        delete [] cur_data;
}

} // namespace Picarus
