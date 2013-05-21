Picarus Takeout

.. image:: https://secure.travis-ci.org/bwhite/picarus_takeout.png
    :target: http://travis-ci.org/bwhite/picarus_takeout
    :alt: Build Status of the master branch

License: Apache 2.0 License

Dependencies
- OpenCV 2.4.5(http://opencv.org/): Image IO, Image Resize, Image Color Convert (BSD License)
- Msgpack(https://github.com/msgpack/msgpack-c): Serialization (Apache 2.0 License)
- Blas(http://math-atlas.sourceforge.net/): Fast math (BSD License)
- fftw3

Included Dependencies (in source)
- GIST: INRIA (GPL License)

Linux Install (Ubuntu)
- Build OpenCV 2.4.4 from source
- Get msgpack (apt-get install libmsgpack-dev)
- Get blas (apt-get install libatlas3gf-base)
- Get fftw3 (apt-get install libfftw3-dev)
- cmake picarus_takeout
- make
