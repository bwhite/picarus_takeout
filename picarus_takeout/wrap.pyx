cimport classes

cdef class ModelChain(object):
    cdef classes.Model *model

    def __init__(self, msgpack_config):
        cdef char *msgpack_config_charp = msgpack_config
        self.model = <classes.Model *> new classes.ModelChain(msgpack_config_charp, len(msgpack_config))
        if self.model is NULL:
            raise ValueError

    def __dealloc__(self):
        del self.model

    def process_binary(self, data):
        cdef char *data_charp = data
        cdef int size_out
        cdef bytes py_string
        cdef int size = len(data)
        cdef unsigned char *output = self.model.process_binary(<unsigned char *>data_charp, size, &size_out)
        if output == NULL:
            raise RuntimeError
        py_string = output[:size_out]  # NOTE: Copies the data
        classes.delete_array(output)
        return py_string
