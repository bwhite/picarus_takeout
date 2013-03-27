#ifndef BOVW_IMAGE_FEATURE
#define BOVW_IMAGE_FEATURE
#include <string>
#include <vector>
#include "Model.hpp"

namespace Picarus {
class BOVWImageFeature: public Model {
private:
    const int num_clusters;
    const int cluster_size;
    const int levels;
    double *clusters;
protected:
    unsigned int *bovw_label_image(double *feature_mask, int height, int width);
public:
    BOVWImageFeature(double *clusters, int num_clusters, int cluster_size, int levels);
    virtual ~BOVWImageFeature();
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};
} // namespace Picarus
#endif
