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
ext_modules = [Extension("_picarus_takeout",
                         ["picarus_takeout/wrap.pyx",
                          "picarus_takeout/picarus_takeout.cpp",
                          "picarus_takeout/ImagePreprocessor.cpp",
                          "picarus_takeout/HistogramImageFeature.cpp",
                          "picarus_takeout/ImageFeature.cpp",
                          "picarus_takeout/picarus_math.c",
                          "picarus_takeout/Model.cpp",
                          "picarus_takeout/ModelChain.cpp",
                          "picarus_takeout/BinaryCollector.cpp",
                          "picarus_takeout/opencv_helpers.cpp",
                          "picarus_takeout/LinearClassifier.cpp",
                          "picarus_takeout/pyramid_histogram_aux.c",
                          "picarus_takeout/bovw_aux.c"],
                         language="c++",
                         include_dirs=['picarus_takeout'],
                         extra_compile_args=['-I', np.get_include(), '-O3', '-Wall', '-mmmx', '-msse', '-msse2', '-DUSE_BLAS'],
                         libraries=['opencv_highgui', 'opencv_core', 'opencv_imgproc', 'blas', 'msgpack'])]

setup(name='picarus_takeout',
      cmdclass=cmdclass,
      version='.01',
      packages=find_packages(),
      ext_modules=ext_modules)
