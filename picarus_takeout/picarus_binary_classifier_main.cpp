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

int read_file(const char *fn, std::string *str) {
    std::ifstream t(fn, std::ios::binary);
    if (!t.is_open())
        return 1;
    t.seekg(0, std::ios::end);   
    str->reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str->assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
    return 0;
}

int write_file(const char *fn, char *data, int size) {
    std::ofstream t(fn, std::ios::out | std::ios::binary);
    if (!t.is_open())
        return 1;
    t.write(data, size);
    t.close();
    return 0;
}

int read_file(const char *fn, std::vector<char> *str) {
    std::ifstream t(fn, std::ios::binary);
    if (!t.is_open())
        return 1;
    t.seekg(0, std::ios::end);   
    str->reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str->assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
    return 0;
}


int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <config_path> <input_path>" << std::endl;
        return 1;
    }
    std::vector<char> msgpack_binary;
    if (read_file(argv[1], &msgpack_binary)) {
        std::cerr << "Could not open: " << argv[1] << std::endl;
        return 1;
    }
        
    std::vector<char> input_data;
    if (read_file(argv[2], &input_data)) {
        std::cerr << "Could not open: " << argv[2] << std::endl;
        return 1;
    }
    std::cout << "Config Size (bytes): " << msgpack_binary.size() << std::endl;
    std::cout << "Input Image Size (bytes): " << input_data.size() << std::endl;

    Picarus::ModelChain mc(&msgpack_binary[0], msgpack_binary.size());

    int size;
    unsigned char *data;
    Picarus::CopyCollector collector(&data, &size);
    mc.process_binary((const unsigned char *)&input_data[0], input_data.size(), &collector);
    if (data == NULL) {
        std::cerr << "Main: ModelChain returned NULL" << std::endl;
    } else {
        double confidence;
        // This decodes the msgpack data, if it isn't a double
        // it will raise an exception
        try {
            Picarus::double_fromstring(data, size, &confidence);
        } catch (msgpack::type_error& e) {
            std::cerr << "Output type is not a double!  This function only accepts models that output doubles, see picarus_takeout_main.cpp to handle arbitrary outputs." << std::endl;
            return 1;
        }
        printf("Confidence[%.15g]\n", confidence);
        delete [] data;
    }
    return 0;
}
