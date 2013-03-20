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

void read_file(const char *fn, std::string *str) {
    std::ifstream t(fn);
    t.seekg(0, std::ios::end);   
    str->reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str->assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
}

void read_file(const char *fn, std::vector<char> *str) {
    std::ifstream t(fn);
    t.seekg(0, std::ios::end);   
    str->reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str->assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
}


int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <config_json_path> <input_path>" << std::endl;
        return 1;
    }
    std::string json_config;
    read_file(argv[1], &json_config);
    std::vector<char> input_data;
    read_file(argv[2], &input_data);

    Picarus::ModelChain mc(json_config.c_str());
    unsigned char *output;
    mc.process_binary((const unsigned char *)&input_data[0], input_data.size(), Picarus::copy_collector, &output);
    double val;
    Picarus::double_fromstring(output + sizeof(int), *((int*)output), &val);
    printf("Confidence[%f]\n", val);
    delete [] output;
    return 0;
}
