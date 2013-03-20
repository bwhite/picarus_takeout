#ifndef PICARUS_MODEL_CHAIN
#define PICARUS_MODEL_CHAIN
#include <vector>
#include "Model.hpp"

namespace Picarus {
class ModelChain : public Model {
protected:
    std::vector<Model*> models;
public:
    ModelChain(const char *json_config);
    virtual ~ModelChain();
    virtual void process_binary(const unsigned char *input, int size, void (*collector)(const unsigned char *, int, void *), void *collector_state);
};
} // namespace Picarus
#endif
