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
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_path> <output_path>" << std::endl;
        return 1;
    }
    std::vector<char> input_data;
    read_file(argv[1], &input_data);
    std::cout << "Input Image Size (bytes): " << input_data.size() << std::endl;
    std::string mode("bgr");
    Picarus::Model *bf = new Picarus::BlocksImageMultiFeature(32, mode, 3);
    int size;
    unsigned char *output = bf->process_binary((const unsigned char *)&input_data[0], input_data.size(), &size);
    write_file(argv[2], (char *)output, size);
    delete [] output;
    delete bf;
}
