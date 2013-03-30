#ifndef BINARY_PREDICTOR
#define BINARY_PREDICTOR
#include "Model.hpp"

namespace Picarus {
class BinaryPredictor : public Model {
private:
    const double threshold;
public:
    BinaryPredictor(double threshold);
    virtual ~BinaryPredictor();
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};

} // namespace Picarus
#endif
