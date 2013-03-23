/*
class NBNNClassifier(MultiClassClassifier):
    """Multi-class classifier using Naive Bayes Nearest Neighbor

    Pros
    - Simple, non-parametric

    Cons
    - Requires NN lookup for # descriptors * # classes which can be slow
    """
    def __init__(self):
        self.db = None
        self.classes = None
        self.dist = distpy.L2Sqr()

    def train(self, label_values):
        self.classes = []
        class_to_num = {}
        self.db = {}  # [class] = features
        # Compute features
        for cur_class, features in label_values:
            if cur_class not in class_to_num:
                class_to_num[cur_class] = len(class_to_num)
                self.classes.append(cur_class)
            cur_class = class_to_num[cur_class]
            if features.ndim == 1:
                print('Skipping due to no features')
                continue
            self.db.setdefault(cur_class, []).append(features)
        for cur_class, features in self.db.items():
            self.db[cur_class] = np.vstack(self.db[cur_class])

    def __call__(self, value):
        class_dists = {}  # [class] = total_dist
        for cur_class in self.db:
            dist_indeces = self.dist.nns(self.db[cur_class], value)
            class_dists[cur_class] = np.sum(dist_indeces[:, 0])
        return [{'class': self.classes[x[0]], 'distance': x[1]} for x in sorted(class_dists.items(),
                                                                                key=lambda x: x[1])]

class LocalNBNNClassifier(NBNNClassifier):
    """Multi-class classifier using Local Naive Bayes Nearest Neighbor

    Pros
    - Simple, non-parametric
    - Faster than NBNN because it only looks at K neighbors (joint space for all classes)

    Cons
    - Requires K-NN lookup for # descriptors
    - Approximates NBNN
    """
    def __init__(self, k=10, *args, **kw):
        super(LocalNBNNClassifier, self).__init__(*args, **kw)
        self.k = k

    def train(self, label_values):
        self.classes = []
        self.class_nums = []
        class_to_num = {}
        self.db = []  # features
        # Compute features
        for cur_class, features in label_values:
            if cur_class not in class_to_num:
                class_to_num[cur_class] = len(class_to_num)
                self.classes.append(cur_class)
            if features.ndim == 1:
                print('Skipping due to no features')
                continue
            self.class_nums += [class_to_num[cur_class]] * len(features)
            self.db.append(features)
        self.db = np.vstack(self.db)
        self.class_nums = np.array(self.class_nums)

    def __call__(self, value):
        class_dists = {}  # [class] = total_dist
        for feature in value:
            dist_indeces = self.dist.knn(self.db, feature, self.k + 1)
            dist_b = dist_indeces[self.k, 0]
            class_min_dists = {}
            for dist, index in dist_indeces[:self.k, :]:
                cur_class = self.class_nums[index]
                class_min_dists[cur_class] = min(class_min_dists.get(cur_class, float('inf')), dist)
            for cur_class, dist_c in class_min_dists.items():
                try:
                    class_dists[cur_class] += dist_c - dist_b
                except KeyError:
                    class_dists[cur_class] = dist_c - dist_b
        return [{'class': self.classes[x[0]], 'distance': x[1]} for x in sorted(class_dists.items(),
                                                                                key=lambda x: x[1])]
 */


#include "LinearClassifier.hpp"
#include <cmath>
#include "picarus_math.h"

namespace Picarus {
LocalNBNNClassifier::LocalNBNNClassifier(double *features, int num_features, int feature_size, const std::vector<std::string> > &labels) :  num_features(num_features), feature_size(feature_size), labels(labels) {
    // TODO: Copy/pack features
}

LocalNBNNClassifier::~LocalNBNNClassifier() {
}

double LocalNBNNClassifier::decision_function(double *feature, int size) {
    // Verify size TODO: Is this how we want to exit if there is an error?
    if (size != coefficients.size())
        return NAN;
    return dot_product(&coefficients[0], feature, size) + intercept;
}

void LocalNBNNClassifier::process_binary(const unsigned char *input, int size, BinaryCollector *collector) {
    std::vector<double> vec;
    std::vector<int> shape;
    ndarray_fromstring(input, size, &vec, &shape);
    double conf = decision_function(&vec[0], vec.size());
    double_tostring(conf, collector);
}
} // namespace Picarus
