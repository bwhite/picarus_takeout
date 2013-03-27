#include "opencv_helpers.hpp"
#include "BOVWImageFeature.hpp"
#include "pyramid_histogram_aux.h"
#include "knearest_neighbor.h"

namespace Picarus {

BOVWImageFeature::BOVWImageFeature(double *clusters, int num_clusters, int cluster_size, int levels) : num_clusters(num_clusters), cluster_size(cluster_size), levels(levels) {
    this->clusters = new double[num_clusters * cluster_size];
    memcpy(this->clusters, clusters, sizeof(double) * num_clusters * cluster_size);
}

BOVWImageFeature::~BOVWImageFeature() {
    delete [] clusters;
}

unsigned int *BOVWImageFeature::bovw_label_image(double *feature_mask, int height, int width) {
    int size = height * width;
    unsigned int *label_image = new unsigned int[size];
    double *neighbor_distances = new double[size];
    nnsl2sqr(feature_mask, clusters, label_image, neighbor_distances, size, num_clusters, cluster_size);
    delete [] neighbor_distances;
    return label_image;
}

void BOVWImageFeature::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    std::vector<double> vec;
    std::vector<int> shape;
    ndarray_fromstring(input, size, &vec, &shape);
    if (shape[2] != cluster_size) {
        (*collector)(NULL, 0);
        return;
    }
    unsigned int *label_image = bovw_label_image(&vec[0], shape[0], shape[1]);
    int feature_size;
    double *feature = pyramid_histogram(label_image, shape[0], shape[1], num_clusters, levels, &feature_size);
    std::vector<double> out_vec(feature, feature + feature_size);  // TODO: Remove copy!
    std::vector<int> out_shape(1);
    out_shape[0] = feature_size;
    ndarray_tostring(out_vec, out_shape, collector);
    delete [] feature;
    delete [] label_image;
}

} // namespace Picarus
