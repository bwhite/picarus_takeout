from setuptools import setup, find_packages
from setuptools.extension import Extension
import re
import numpy as np


def get_cython_version():
    """
    Returns:
        Version as a pair of ints (major, minor)

    Raises:
        ImportError: Can't load cython or find version
    """
    import Cython.Compiler.Main
    match = re.search('^([0-9]+)\.([0-9]+)',
                      Cython.Compiler.Main.Version.version)
    try:
        return map(int, match.groups())
    except AttributeError:
        raise ImportError

# Only use Cython if it is available, else just use the pre-generated files
try:
    cython_version = get_cython_version()
    # Requires Cython version 0.13 and up
    if cython_version[0] == 0 and cython_version[1] < 13:
        raise ImportError
    from Cython.Distutils import build_ext
    source_ext = '.pyx'
    cmdclass = {'build_ext': build_ext}
except ImportError:
    source_ext = '.c'
    cmdclass = {}

#  '-ftree-vectorizer-verbose=2'
#, '-lblas' '-lopencv_highgui', '-lopencv_core','-lopencv_imgproc'
SOURCES = 'LocalNBNNClassifier.cpp knearest_neighbor.c opencv_helpers.cpp PixelsImageFeature.cpp ImageMaskFeature.cpp ImageMultiFeature.cpp BinaryCollector.cpp BlocksImageMultiFeature.cpp picarus_takeout.cpp HOGImageMaskFeature.cpp features.cc ModelChain.cpp Model.cpp picarus_takeout_main.cpp ImagePreprocessor.cpp BOVWImageFeature.cpp HistogramImageFeature.cpp ImageFeature.cpp BinaryPredictor.cpp picarus_math.c LinearClassifier.cpp pyramid_histogram_aux.c bovw_aux.c GISTImageFeature.cpp gist/gist.c gist/gist_wrapper.c gist/standalone_image.c hamming_aux.c HashIndex.cpp SphericalHashIndex.cpp FaceImageObjectDetector.cpp ImageObjectDetector.cpp Hasher.cpp SphericalHasher.cpp'
SOURCES = ['picarus_takeout/' + x for x in SOURCES.split()]
ext_modules = [Extension("_picarus_takeout",
                         ["picarus_takeout/wrap.pyx"] + SOURCES,
                         language="c++",
                         include_dirs=['picarus_takeout'],
                         extra_compile_args=['-I', np.get_include(), '-O3', '-Wall', '-mmmx', '-msse', '-msse2', '-DUSE_BLAS'],
                         libraries=['opencv_highgui', 'opencv_core', 'opencv_imgproc', 'opencv_objdetect', 'blas', 'msgpack', 'fftw3f'])]

setup(name='picarus_takeout',
      cmdclass=cmdclass,
      version='.01',
      packages=find_packages(),
      ext_modules=ext_modules)
