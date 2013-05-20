set_source_files_properties(picarus_takeout.pyx PROPERTIES CYTHON_IS_CXX TRUE)

IF(CMAKE_BUILD_TYPE MATCHES debug)
    message("debug mode")
    add_definitions(-DDEBUG)
    if(CMAKE_COMPILER_IS_GNUCXX)
      set(CMAKE_CXX_FLAGS "-g -O2 -Wall -fPIC")
    endif()
    if(CMAKE_COMPILER_IS_GNUC)
      set(CMAKE_C_FLAGS "-g -O2 -Wall -fPIC")
    endif()
ENDIF(CMAKE_BUILD_TYPE MATCHES debug)

IF(CMAKE_BUILD_TYPE MATCHES release)
    message("release mode")
    add_definitions(-DNDEBUG)
    if(CMAKE_COMPILER_IS_GNUCXX)
      set(CMAKE_CXX_FLAGS "-O3 -Wall -fPIC -mmmx -msse -msse2")
    endif()
    if(CMAKE_COMPILER_IS_GNUC)
      set(CMAKE_C_FLAGS "-O3 -Wall -fPIC -mmmx -msse -msse2")
    endif()
ENDIF(CMAKE_BUILD_TYPE MATCHES release)

execute_process ( COMMAND python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)

add_executable(picarus LocalNBNNClassifier.cpp knearest_neighbor.c opencv_helpers.cpp PixelsImageFeature.cpp ImageMaskFeature.cpp ImageMultiFeature.cpp FaceImageObjectDetector.cpp ImageObjectDetector.cpp BinaryCollector.cpp BlocksImageMultiFeature.cpp BOVWImageFeature.cpp picarus_takeout.cpp HOGImageMaskFeature.cpp features.cc ModelChain.cpp Model.cpp picarus_takeout_main.cpp ImagePreprocessor.cpp BinaryPredictor.cpp HistogramImageFeature.cpp ImageFeature.cpp picarus_math.c LinearClassifier.cpp ImageFeature2d.cpp BRISKImageFeature2d.cpp HammingFeature2dHashIndex.cpp pyramid_histogram_aux.c bovw_aux.c GISTImageFeature.cpp HashIndex.cpp SphericalHashIndex.cpp hamming_aux.c SphericalHasher.cpp Hasher.cpp gist/gist.c gist/gist_wrapper.c gist/standalone_image.c multi_index.c HammingMultiIndex.cpp KernelClassifier.cpp kernels_aux.c)

add_executable(picarus_binary_classifier LocalNBNNClassifier.cpp knearest_neighbor.c opencv_helpers.cpp PixelsImageFeature.cpp ImageMaskFeature.cpp ImageMultiFeature.cpp FaceImageObjectDetector.cpp ImageObjectDetector.cpp BinaryCollector.cpp BlocksImageMultiFeature.cpp BOVWImageFeature.cpp picarus_takeout.cpp HOGImageMaskFeature.cpp features.cc ModelChain.cpp Model.cpp picarus_binary_classifier_main.cpp ImagePreprocessor.cpp BinaryPredictor.cpp HistogramImageFeature.cpp ImageFeature.cpp picarus_math.c LinearClassifier.cpp ImageFeature2d.cpp BRISKImageFeature2d.cpp HammingFeature2dHashIndex.cpp pyramid_histogram_aux.c bovw_aux.c GISTImageFeature.cpp HashIndex.cpp SphericalHashIndex.cpp hamming_aux.c SphericalHasher.cpp Hasher.cpp gist/gist.c gist/gist_wrapper.c gist/standalone_image.c multi_index.c HammingMultiIndex.cpp KernelClassifier.cpp kernels_aux.c)

if(NOT WIN32)
  add_executable(spherical_hasher_train spherical_hasher_train.c spherical_hasher_train_main.c)
  add_definitions(-DUSE_BLAS)
endif()
if(WIN32)
  target_link_libraries(picarus msgpackd opencv_highgui245 opencv_core245 opencv_imgproc245 opencv_objdetect245 opencv_features2d245 libfftw3f-3 ws2_32 pthreadVC2)
  target_link_libraries(picarus_binary_classifier msgpackd opencv_highgui245 opencv_core245 opencv_imgproc245 opencv_objdetect245 opencv_features2d245 libfftw3f-3 ws2_32 pthreadVC2)
else()
  target_link_libraries(picarus opencv_highgui opencv_core opencv_imgproc opencv_objdetect opencv_features2d blas msgpack fftw3f)
  target_link_libraries(picarus_binary_classifier opencv_highgui opencv_core opencv_imgproc opencv_objdetect opencv_features2d blas msgpack fftw3f)
endif()

cython_add_module(picarus_takeout picarus_takeout.pyx LocalNBNNClassifier.cpp knearest_neighbor.c opencv_helpers.cpp PixelsImageFeature.cpp ImageMaskFeature.cpp ImageMultiFeature.cpp FaceImageObjectDetector.cpp ImageObjectDetector.cpp BinaryCollector.cpp BlocksImageMultiFeature.cpp BOVWImageFeature.cpp picarus_takeout.cpp HOGImageMaskFeature.cpp features.cc ModelChain.cpp Model.cpp ImagePreprocessor.cpp BinaryPredictor.cpp HistogramImageFeature.cpp ImageFeature.cpp picarus_math.c LinearClassifier.cpp ImageFeature2d.cpp BRISKImageFeature2d.cpp HammingFeature2dHashIndex.cpp pyramid_histogram_aux.c bovw_aux.c GISTImageFeature.cpp HashIndex.cpp SphericalHashIndex.cpp hamming_aux.c SphericalHasher.cpp Hasher.cpp gist/gist.c gist/gist_wrapper.c gist/standalone_image.c multi_index.c HammingMultiIndex.cpp KernelClassifier.cpp kernels_aux.c spherical_hasher_train.c)
target_link_libraries(picarus_takeout opencv_highgui opencv_core opencv_imgproc opencv_objdetect opencv_features2d blas msgpack fftw3f)

INSTALL(TARGETS picarus_takeout picarus
  RUNTIME DESTINATION /usr/local/bin
  LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES}
)