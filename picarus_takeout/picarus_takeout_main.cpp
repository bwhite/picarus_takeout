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

void read_file(const char *fn, std::vector<char> *str) {
    std::ifstream t(fn, std::ios::binary);
    t.seekg(0, std::ios::end);   
    str->reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str->assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
}


int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <config_json_path> <input_path> <output_path>" << std::endl;
        return 1;
    }
    std::vector<char> msgpack_binary;
    read_file(argv[1], &msgpack_binary);
    std::vector<char> input_data;
    read_file(argv[2], &input_data);
    std::cout << "JSON Config Size (bytes): " << msgpack_binary.size() << std::endl;
    std::cout << "Input Image Size (bytes): " << input_data.size() << std::endl;

    Picarus::ModelChain mc(&msgpack_binary[0], msgpack_binary.size());

    int size;
    unsigned char *data;
    Picarus::CopyCollector collector(&data, &size);
    mc.process_binary((const unsigned char *)&input_data[0], input_data.size(), &collector);
    if (data == NULL) {
        printf("Main: ModelChain returned NULL\n");
    } else {
        std::vector<double> vec;
        std::vector<int> shape;
        Picarus::ndarray_fromstring(data, size, &vec, &shape);
        for (int i = 0; i < shape.size(); ++i)
            printf("%d\n", shape[i]);
        write_file(argv[3], (char *)data, size);
        /*for (int i = 0; i < vec.size(); ++i)
          printf("%f\n", vec[i]); */
        delete [] data;

        /*
        std::vector<std::pair<double, std::string> > val;
        Picarus::double_strings_fromstring(data, size, &val);
        for (int i = 0; i < val.size(); ++i)
            printf("%s %f\n", val[i].second.c_str(), val[i].first);
        delete [] data;
        */
        /*
        printf("Confidence[%f]\n", val);
        double val;
        Picarus::double_fromstring(data, size, &val);
        printf("Confidence[%f]\n", val);
        delete [] data;
        */
    }
    return 0;
}
