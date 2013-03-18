#import cv
import numpy as np
cimport numpy as np
#import imfeat
#cimport imfeat
import json
cimport classes
from cpython cimport PyObject, Py_INCREF


cdef extern from "string.h":
    void *memcpy(void *dest, void *src, int n)

cdef extern from "stdlib.h":
    void free(void *ptr)


cdef extern from "numpy/arrayobject.h":
    void import_array()
    cdef object PyArray_SimpleNewFromData(int nd, np.npy_intp *dims,
                                          int typenum, void *data)

import_array()


cdef class PicarusArrayWrap:
    cdef void* data_ptr
    cdef np.ndarray dims
    cdef int typenum

    cdef set_data(self, dims, void* data_ptr, int typenum):
        self.data_ptr = data_ptr
        self.dims = np.array(dims, dtype=np.long)
        self.typenum = typenum

    def __array__(self):
        return np.PyArray_SimpleNewFromData(self.dims.size, <np.npy_intp *>self.dims.data,
                                            self.typenum, self.data_ptr)

    def __dealloc__(self):
        classes.picarus_delete_array(self.data_ptr)


cdef object wrap_array(object dims, void *array, int typenum):
    cdef np.ndarray ndarray
    wrap = PicarusArrayWrap()
    wrap.set_data(dims, array, typenum)
    ndarray = np.array(wrap, copy=False)
    ndarray.base = <PyObject *>wrap
    Py_INCREF(wrap)
    return ndarray

cdef class ImagePreprocessor(object):
    cdef classes.ImagePreprocessor *ip

    def __init__(self, **kw):
        json_config = json.dumps({'name': 'picarus.ImagePreprocessor', 'kw': kw})
        cdef char *json_config_charp = json_config
        cdef classes.cJSON *config = classes.cJSON_Parse(json_config_charp)
        self.ip = classes.picarus_image_preprocessor_factory(config)
        if self.ip is NULL:
            raise ValueError

    def asbinary(self, binary_image):
        cdef unsigned char *binary_image_ptr = binary_image
        cdef int size_out
        cdef bytes py_string
        cdef unsigned char *image_data = self.ip.asbinary(binary_image_ptr, len(binary_image), &size_out)
        if image_data == NULL:
            raise RuntimeError
        py_string = image_data[:size_out]  # NOTE: Copies the data
        classes.picarus_delete_array(image_data)
        return py_string
    
    def asarray(self, binary_image):
        cdef unsigned char *binary_image_ptr = binary_image
        cdef int height, width, channels
        cdef unsigned char *image_data = self.ip.asarray(binary_image_ptr, len(binary_image), &height, &width, &channels)
        if image_data == NULL:
            raise RuntimeError
        return wrap_array([height, width, channels], image_data, np.NPY_UINT8)

cdef class HistogramImageFeature(object):
    cdef classes.HistogramImageFeature *hif

    def __init__(self, **kw):
        json_config = json.dumps({'name': 'picarus.HistogramImageFeature', 'kw': kw})
        cdef char *json_config_charp = json_config
        cdef classes.cJSON *config = classes.cJSON_Parse(json_config_charp)
        self.hif = classes.picarus_histogram_image_feature_factory(config)
        if self.hif is NULL:
            raise ValueError

    def compute_feature(self, np.ndarray image):
        if image.ndim != 3 or image.shape[2] != 3:
            raise ValueError('Expecting BGR image')
        cdef int size
        cdef double *feat = self.hif.compute_feature(<unsigned char *>image.data, image.shape[0], image.shape[1], &size)
        return wrap_array([size], feat, np.NPY_DOUBLE)

cdef class LinearClassifier(object):
    cdef classes.LinearClassifier *lc

    def __init__(self, **kw):
        json_config = json.dumps({'name': 'picarus.LinearClassifier', 'kw': kw})
        cdef char *json_config_charp = json_config
        cdef classes.cJSON *config = classes.cJSON_Parse(json_config_charp)
        self.lc = classes.picarus_linear_classifier_factory(config)
        if self.lc is NULL:
            raise ValueError

    def decision_function(self, np.ndarray feature):
        if feature.ndim != 1 or feature.dtype != np.float64:
            raise ValueError('Feature must be a vector of doubles')
        return self.lc.decision_function(<double *>feature.data, feature.size)
