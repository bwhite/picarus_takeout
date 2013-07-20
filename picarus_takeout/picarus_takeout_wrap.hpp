#ifndef PICARUS_TAKEOUT_WRAP
#define PICARUS_TAKEOUT_WRAP
#ifdef __cplusplus
extern "C" {
#endif
    void *picarus_takeout_new_model_chain(const char *msgpack_binary, int size);
    char *picarus_takeout_process_binary(void *model, const char *input, int size, int *size_out);
    void picarus_takeout_delete_model(void *model);
    void picarus_takeout_delete_output(void *array);
    char *picarus_takeout_model_chain_process_binary(const char *msgpack_binary, int size, const char *input, int input_size, int *size_out);
#ifdef __cplusplus
}
#endif
#endif
