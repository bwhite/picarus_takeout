#include "LocalNBNNClassifier.hpp"
#include <cmath>
#include <algorithm>
#include "picarus_math.h"
#include "knearest_neighbor.h"
#include <cstring>
#include <stdexcept>

namespace Picarus {
LocalNBNNClassifier::LocalNBNNClassifier(double *features, int *indeces, int num_features, int feature_size, const std::vector<std::string> &labels, int max_results) :  num_features(num_features), feature_size(feature_size), labels(labels), max_results(max_results) {
    this->features = new double[num_features * feature_size];
    this->indeces = new int[num_features];
    memcpy(this->features, features, num_features * feature_size * sizeof(double));
    memcpy(this->indeces, indeces, num_features * sizeof(int));
}

LocalNBNNClassifier::~LocalNBNNClassifier() {
    delete [] this->features;
    delete [] this->indeces;
}


std::map<int, double> *LocalNBNNClassifier::knn(double *feature, int feature_size) {
    if (feature_size != this->feature_size)
        return NULL;
    int *neighbor_indeces = new int[max_results + 1];
    double *neighbor_dists = new double[max_results + 1];
    std::map<int, double> *class_min_dists = new std::map<int, double>();
    int total_results = knnl2sqr(feature, this->features, neighbor_indeces,
                                 neighbor_dists, this->num_features, feature_size,
                                 max_results + 1);
    for (int i = 0; i < total_results - 1; ++i) {
        double prev_val;
        try {
            prev_val = (*class_min_dists).at(neighbor_indeces[i]);
        } catch (const std::out_of_range& oor) {
            prev_val = INFINITY;
        }
        double cur_val = neighbor_dists[i] - neighbor_dists[total_results - 1];
        if (cur_val < prev_val)
            (*class_min_dists)[neighbor_indeces[i]] = cur_val;
    }
    delete [] neighbor_indeces;
    delete [] neighbor_dists;
    return class_min_dists;
}

std::vector<std::pair<double, std::string> > *LocalNBNNClassifier::classify(double *features, int num_features, int feature_size) {
    if (this->feature_size != feature_size) {
        return NULL;
    }
    std::map<std::string, double> class_dists;
    for (int i = 0; i < num_features; ++i, features += feature_size) {
        // Compute K-NN between current input feature and all DB features
        std::map<int, double> *class_min_dists = knn(features, feature_size);
        if (class_min_dists == NULL) {
            return NULL;
        }
        // Update class dists with K-NN Results
        std::map<int, double>::const_iterator itr;
        for (itr = class_min_dists->begin(); itr != class_min_dists->end(); ++itr) {
            std::string name = labels[indeces[itr->first]];
            class_dists[name] = class_dists[name] + itr->second;
        }
        delete class_min_dists;
    }
    std::vector<std::pair<double, std::string> > *class_dists_out  = new std::vector<std::pair<double, std::string> >();
    class_dists_out->reserve(class_dists.size());
    std::map<std::string, double>::const_iterator itr;
    for (itr = class_dists.begin(); itr != class_dists.end(); ++itr)
        class_dists_out->push_back(std::make_pair(itr->second, itr->first));
    std::sort(class_dists_out->begin(), class_dists_out->end());
    return class_dists_out;
}

void LocalNBNNClassifier::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    std::vector<double> vec;
    std::vector<int> shape;
    ndarray_fromstring(input, size, &vec, &shape);
    std::vector<std::pair<double, std::string> > *class_dists = classify(&vec[0], shape[0], shape[1]);
    if (class_dists == NULL) {
        (*collector)(NULL, 0);
        return;
    }
    double_strings_tostring(*class_dists, collector);
    delete class_dists;
}
} // namespace Picarus
