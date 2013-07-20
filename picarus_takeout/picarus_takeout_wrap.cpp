#include "picarus_takeout_wrap.hpp"
#include "ModelChain.hpp"
#include "picarus_takeout.hpp"

void *picarus_takeout_new_model_chain(const char *msgpack_binary, int size) {
    return new Picarus::ModelChain(msgpack_binary, size);
}

char *picarus_takeout_model_chain_process_binary(const char *msgpack_binary, int size, const char *input, int input_size, int *size_out) {
    void *model = picarus_takeout_new_model_chain(msgpack_binary, size);
    char *out = picarus_takeout_process_binary(model, input, input_size, size_out);
    picarus_takeout_delete_model(model);
    return out;
}

char *picarus_takeout_process_binary(void *model, const char *input, int size, int *size_out) {
    return (char *)((Picarus::Model*)model)->process_binary((const unsigned char *)input, size, size_out);
}

void picarus_takeout_delete_model(void *model) {
    delete (Picarus::Model*)model;
}

void picarus_takeout_delete_output(void *array) {
    Picarus::delete_array(array);
}
