#ifndef PICARUS_MODEL_CHAIN
#define PICARUS_MODEL_CHAIN
#include <vector>
#include "Model.hpp"

namespace Picarus {
class ModelChain : public Model {
protected:
    std::vector<Model*> models;
public:
    ModelChain(const char *msgpack_binary, int size);
    virtual ~ModelChain();
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};
} // namespace Picarus
#endif
