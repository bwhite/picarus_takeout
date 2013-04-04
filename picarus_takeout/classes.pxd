cdef extern from "picarus_takeout.hpp" namespace "Picarus":
    void delete_array(void *image)

cdef extern from "Model.hpp" namespace "Picarus":
    cdef cppclass Model:
        unsigned char *process_binary(unsigned char *input, int size, int *size_out)

cdef extern from "ModelChain.hpp" namespace "Picarus":
    cdef cppclass ModelChain:
        ModelChain(char *, int)
        unsigned char *process_binary(unsigned char *input, int size, int *size_out)

cdef extern from "spherical_hasher_train.h":
    void spherical_hasher_train(double *points, double *pivots, double *threshs, int num_points, int num_dims, int num_pivots,
                                double eps_m, double eps_s, int max_iters)
