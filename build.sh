rm *.o picarus
PARAMS="-Wall -Wextra -g -DUSE_BLAS -lblas -lmsgpack -lprofiler"

g++ -o picarus picarus_takeout/picarus_takeout.cpp picarus_takeout/ModelChain.cpp picarus_takeout/Model.cpp picarus_takeout/picarus_takeout_main.cpp picarus_takeout/cJSON.c picarus_takeout/ImagePreprocessor.cpp picarus_takeout/HistogramImageFeature.cpp picarus_takeout/ImageFeature.cpp picarus_takeout/picarus_math.c picarus_takeout/LinearClassifier.cpp picarus_takeout/pyramid_histogram_aux.c picarus_takeout/bovw_aux.c ${PARAMS} -l opencv_highgui -l opencv_core -l opencv_imgproc