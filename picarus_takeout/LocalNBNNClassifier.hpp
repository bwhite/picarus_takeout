#ifndef LOCAL_NBNN_CLASSIFIER
#define LOCAL_NBNN_CLASSIFIER
#include <vector>
#include "Model.hpp"

namespace Picarus {
class LocalNBNNClassifier : public Model {
private:
    const int num_features;
    const int feature_size;
    double *features;
    int *indeces;
    const std::vector<std::string> labels;
    const int max_results;
protected:
    std::map<int, double> *knn(double *feature, int feature_size);
public:
    LocalNBNNClassifier(double *features, int *indeces, int num_features, int feature_size, const std::vector<std::string> &labels, int max_results);
    virtual ~LocalNBNNClassifier();
    std::vector<std::pair<double, std::string> > *classify(double *features, int num_features, int feature_size);
    virtual void process_binary(const unsigned char *input, int size, BinaryCollector *collector);
};

} // namespace Picarus
#endif
