#import cv
#import numpy as np
#cimport numpy as np
#import imfeat
#cimport imfeat
import json
cimport classes


cdef class ImagePreprocessor(object):
    cdef classes.ImagePreprocessor *ip

    def __init__(self, **kw):
        json_config = json.dumps({'name': 'picarus.ImagePreprocessor', 'kw': kw})
        cdef char *json_config_charp = json_config
        cdef classes.cJSON *config = classes.cJSON_Parse(json_config_charp)
        self.ip = classes.picarus_image_preprocessor_factory(config)
        if self.ip is NULL:
            raise ValueError
    
    def asarray(self, binary_image):
        cdef unsigned char *binary_image_ptr = binary_image
        cdef int height, width, channels
        cdef unsigned char *image = self.ip.asarray(binary_image_ptr, len(binary_image), &height, &width, &channels)
