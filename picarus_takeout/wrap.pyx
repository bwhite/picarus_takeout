cimport classes
import numpy as np
cimport numpy as np
import random

np.import_array()

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

def spherical_hasher_train(points, num_pivots, eps_m, eps_s, max_iters):
    cdef np.ndarray points_array = np.ascontiguousarray(points)
    num_pivots = min(points_array.shape[0], num_pivots)
    cdef np.ndarray pivots = np.ascontiguousarray(random.sample(points, num_pivots)).copy()
    #cdef np.ndarray pivots = np.ascontiguousarray(points[:num_pivots, :]).copy()
    cdef np.ndarray threshs = np.zeros(num_pivots)
    classes.spherical_hasher_train(<np.double_t *>points_array.data, <np.double_t *>pivots.data, <np.double_t *>threshs.data,
                                   points_array.shape[0], points_array.shape[1], num_pivots, eps_m, eps_s, max_iters)
    return pivots, threshs
