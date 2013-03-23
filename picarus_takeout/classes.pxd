cdef extern from "picarus_takeout.hpp" namespace "Picarus":
    void delete_array(void *image)

cdef extern from "Model.hpp" namespace "Picarus":
    void double_fromstring(unsigned char *input, int size, double *val)
    cdef cppclass Model:
        unsigned char *process_binary(unsigned char *input, int size, int *size_out)

cdef extern from "ModelChain.hpp" namespace "Picarus":
    cdef cppclass ModelChain:
        ModelChain(char *)
        unsigned char *process_binary(unsigned char *input, int size, int *size_out)
