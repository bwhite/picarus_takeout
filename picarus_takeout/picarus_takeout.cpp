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
} // namespace Picarus
