cdef extern from "cJSON.h":
    ctypedef struct cJSON:
        pass
    cJSON *cJSON_Parse(char *value)


cdef extern from "picarus_takeout.hpp" namespace "Picarus":
    cdef cppclass ImagePreprocessor:
        unsigned char *asbinary(unsigned char *binary_image, int size, int *size_out)

    cdef cppclass HistogramImageFeature:
        double *compute_feature(unsigned char *image, int height, int width, int *out_size)

    cdef cppclass LinearClassifier:
        double decision_function(double *feature, int size)

    ImagePreprocessor* image_preprocessor_factory(cJSON *cjs)
    void delete_array(void *image)
    HistogramImageFeature* histogram_image_feature_factory(cJSON *cjs)
    LinearClassifier* linear_classifier_factory(cJSON *cjs)                                                                                                                
