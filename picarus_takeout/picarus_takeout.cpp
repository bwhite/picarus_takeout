#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cassert>
#include "picarus_takeout.hpp"

namespace Picarus {

void delete_array(void *array) {
    delete [] array;
}

ImagePreprocessor* image_preprocessor_factory(std::map<std::string, msgpack::object> *kw) {
    std::string method, compression;
    int size;
    kw->at(std::string("method")) >> method;
    kw->at(std::string("compression")) >> compression;
    kw->at(std::string("size")) >> size;
    return new ImagePreprocessor(method, size, compression);
    // TODO: Check bounds/params
}

HistogramImageFeature* histogram_image_feature_factory(std::map<std::string, msgpack::object> *kw) {
    std::string mode;
    int levels;
    std::vector<int> num_bins;
    kw->at(std::string("mode")) >> mode;
    kw->at(std::string("levels")) >> levels;
    try {
        kw->at(std::string("num_bins")) >> num_bins;
    } catch (msgpack::type_error&) {
        int num_bins_int;
        kw->at(std::string("num_bins")) >> num_bins_int;
        num_bins.resize(3);
        for (int i = 0; i < 3; ++i)
            num_bins[i] = num_bins_int;
    }
    // TODO: Check bounds/params
    return new HistogramImageFeature(mode, num_bins, levels);
}

LinearClassifier* linear_classifier_factory(std::map<std::string, msgpack::object> *kw) {
    std::vector<double> coefficients;
    double intercept;
    kw->at(std::string("coefficients")) >> coefficients;
    kw->at(std::string("intercept")) >> intercept;

    // TODO: Check bounds/params
    return new LinearClassifier(&coefficients[0], coefficients.size(), intercept);
}


BlocksImageMultiFeature* blocks_image_multi_feature_factory(std::map<std::string, msgpack::object> *kw) {
    int block_size, levels;
    std::string mode;
    kw->at(std::string("block_size")) >> block_size;
    kw->at(std::string("mode")) >> mode;
    kw->at(std::string("levels")) >> levels;

    // TODO: Check bounds/params
    return new BlocksImageMultiFeature(block_size, mode, levels);
}

LocalNBNNClassifier* local_nbnn_classifier_factory(std::map<std::string, msgpack::object> *kw) {
    std::vector<double> features;
    std::vector<int> indeces;
    double feature_size;
    std::vector<std::string> labels;
    int max_results;

    kw->at(std::string("features")) >> features;
    kw->at(std::string("indeces")) >> indeces;
    kw->at(std::string("feature_size")) >> feature_size;
    kw->at(std::string("labels")) >> labels;
    kw->at(std::string("max_results")) >> max_results;

    // TODO: Check bounds/params
    return new LocalNBNNClassifier(&features[0], &indeces[0], indeces.size() / feature_size, feature_size, labels, max_results);
}
} // namespace Picarus
