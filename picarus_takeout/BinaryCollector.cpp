#include "BinaryCollector.hpp"
#include <cstring>
namespace Picarus {
BinaryCollector::BinaryCollector() {}
BinaryCollector::~BinaryCollector() {}
CopyCollector::CopyCollector(unsigned char **output, int *size) : output(output), size(size) {
}

CopyCollector::~CopyCollector() {
}

void CopyCollector::operator()(const unsigned char *output, int size) {
    if (output == NULL) {
        *(this->output) = NULL;
        *(this->size) = 0;
        return;
    }
    *(this->output) = new unsigned char[size];
    *(this->size) = size;
    memcpy(*(this->output), output, size);
    
}
} // namespace Picarus
