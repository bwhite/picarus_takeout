#include "BinaryPredictor.hpp"

namespace Picarus {
BinaryPredictor::BinaryPredictor(double threshold) :  threshold(threshold) {
}

BinaryPredictor::~BinaryPredictor() {
}

void BinaryPredictor::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    double confidence;
    double_fromstring(input, size, &confidence);
    bool_tostring(confidence >= threshold, collector);    
}
} // namespace Picarus
