cdef extern from "cJSON.h":
    ctypedef struct cJSON:
        pass
    cJSON *cJSON_Parse(char *value)


cdef extern from "picarus_takeout.hpp":
    cdef cppclass ImagePreprocessor:
        unsigned char *asarray(unsigned char *binary_image, int size, int *height_out, int *width_out, int *channels_out)
    ImagePreprocessor* picarus_image_preprocessor_factory(cJSON *cjs)
    #HistogramImageFeature* picarus_histogram_image_feature_factory(cJSON *cjs)
    #LinearClassifier* picarus_linear_classifier_factory(cJSON *cjs)                                                                                                                
