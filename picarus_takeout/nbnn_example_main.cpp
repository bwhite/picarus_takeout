#include <fstream>
#include <streambuf>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cassert>
#include "picarus_takeout.hpp"
#include "ModelChain.hpp"
#include "BlocksImageMultiFeature.hpp"
#include "Model.hpp"
#include "LocalNBNNClassifier.hpp"
#include <msgpack.hpp>

void read_file(const char *fn, std::string *str) {
    std::ifstream t(fn, std::ios::binary);
    t.seekg(0, std::ios::end);   
    str->reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str->assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
}

void read_file(const char *fn, std::vector<char> *str) {
    std::ifstream t(fn, std::ios::binary);
    t.seekg(0, std::ios::end);   
    str->reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str->assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
}

void write_file(const char *fn, char *data, int size) {
    std::ofstream t(fn, std::ios::out | std::ios::binary);
    t.write(data, size);
    t.close();
}



int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_paths>..." << std::endl;
        return 1;
    }

    std::string mode("bgr");
    Picarus::Model *bf = new Picarus::BlocksImageMultiFeature(20, mode, 3);
    Picarus::Model *pp = new Picarus::ImagePreprocessor(std::string("force_max_side"), 80, std::string("jpg"));

    // Build database
    std::vector<std::string> labels;
    std::vector<double> features;
    std::vector<int> indeces;
    int num_features = 0;
    int feature_size;
    for (int i = 0; i < argc - 1; ++i) {
        labels.push_back(std::string(argv[i + 1]));
        std::vector<char> input_data;
        read_file(argv[i + 1], &input_data);

        int size3;
        unsigned char *output3 = pp->process_binary((const unsigned char *)&input_data[0], input_data.size(), &size3);

        int size;
        unsigned char *output = bf->process_binary(output3, size3, &size);
        std::vector<double> vec;
        std::vector<int> shape;
        Picarus::ndarray_fromstring(output, size, &vec, &shape);
        feature_size = shape[1];
        num_features += shape[0];
        for (int j = 0; j < shape[0]; ++j)
            indeces.push_back(i);
        features.insert(features.end(), vec.begin(), vec.end());
        delete [] output;
        delete [] output3;
    }

    Picarus::Model *nbnn = new Picarus::LocalNBNNClassifier(&features[0], &indeces[0], num_features, feature_size, labels, 5);

    for (int i = 0; i < argc - 1; ++i) {
        std::vector<char> input_data;
        read_file(argv[i + 1], &input_data);

        int size3;
        unsigned char *output3 = pp->process_binary((const unsigned char *)&input_data[0], input_data.size(), &size3);

        int size;
        unsigned char *output = bf->process_binary(output3, size3, &size);
        int size2;
        unsigned char *output2 = nbnn->process_binary(output, size, &size2);
        delete [] output;
        delete [] output2;
        delete [] output3;
    }
    delete nbnn;
    delete bf;
    delete pp;
}
