try:
    import unittest2 as unittest
except ImportError:
    import unittest
#import _picarus_takeout as picarus_takeout
import picarus_takeout
import numpy as np
import glob
# Cheat Sheet (method/test) <http://docs.python.org/library/unittest.html>
#
# assertEqual(a, b)       a == b   
# assertNotEqual(a, b)    a != b    
# assertTrue(x)     bool(x) is True  
# assertFalse(x)    bool(x) is False  
# assertRaises(exc, fun, *args, **kwds) fun(*args, **kwds) raises exc
# assertAlmostEqual(a, b)  round(a-b, 7) == 0         
# assertNotAlmostEqual(a, b)          round(a-b, 7) != 0
# 
# Python 2.7+ (or using unittest2)
#
# assertIs(a, b)  a is b
# assertIsNot(a, b) a is not b
# assertIsNone(x)   x is None
# assertIsNotNone(x)  x is not None
# assertIn(a, b)      a in b
# assertNotIn(a, b)   a not in b
# assertIsInstance(a, b)    isinstance(a, b)
# assertNotIsInstance(a, b) not isinstance(a, b)
# assertRaisesRegexp(exc, re, fun, *args, **kwds) fun(*args, **kwds) raises exc and the message matches re
# assertGreater(a, b)       a > b
# assertGreaterEqual(a, b)  a >= b
# assertLess(a, b)      a < b
# assertLessEqual(a, b) a <= b
# assertRegexpMatches(s, re) regex.search(s)
# assertNotRegexpMatches(s, re)  not regex.search(s)
# assertItemsEqual(a, b)    sorted(a) == sorted(b) and works with unhashable objs
# assertDictContainsSubset(a, b)      all the key/value pairs in a exist in b

try:
    import imfeat
except ImportError:
    print('Skipping imfeat tests due to ImportError...')
    imfeat = None


class Test(unittest.TestCase):

    def setUp(self):
        self.features = []
        self.features.append(picarus_takeout.HistogramImageFeature(mode='lab', levels=1, num_bins=[8, 8, 8]))
        self.features.append(picarus_takeout.HistogramImageFeature(mode='lab', levels=3, num_bins=[1, 1, 1]))
        self.features.append(picarus_takeout.HistogramImageFeature(mode='bgr', levels=1, num_bins=[8, 8, 8]))
        self.features.append(picarus_takeout.HistogramImageFeature(mode='rgb', levels=1, num_bins=[8, 8, 8]))

    def tearDown(self):
        pass

    def _feature_tests(self, image):
        for feature in self.features:
            feature.compute_feature(image)

    def _run_test(self, method):
        compression = '.jpg'
        sizes = [1, 2, 3, 5, 50, 64, 100, 101, 150, 250, 500]
        for x in glob.glob('test_images/*'):
            if x.endswith('.gif'):  # Not supported
                continue
            data_binary = open(x).read()
            for size in sizes:
                print((x, method, size))
                ip = picarus_takeout.ImagePreprocessor(method=method, size=size, compression=compression)
                image = ip.asarray(data_binary)
                #if imfeat is not None:
                #    ip_imfeat = imfeat.ImagePreprocessor(method=method, size=size, compression=compression)
                #    image_imfeat = ip_imfeat.asarray(data_binary)
                #    np.testing.assert_equal(image, image_imfeat)
                # Idempotence
                for _ in range(1000):
                    image_binary = ip.asbinary(open(x).read())
                image2 = ip.asarray(image_binary)
                #image_binary2 = ip.asbinary(image_binary)
                #self.assertEquals(image_binary, image_binary2)
                #self.assertEquals(image.ndim, 3)
                print np.median(np.abs((image - image2).ravel()))
                #, 5)self.assertLess(
                yield image, size
                yield image2, size
                self._feature_tests(image)
                

    def test_max_size(self):
        for image, size in self._run_test('max_side'):
            self.assertLessEqual(np.max(image.shape[:2]), size)

    def test_force_max_size(self):
        for image, size in self._run_test('force_max_side'):
            self.assertEqual(np.max(image.shape[:2]), size)

    def test_force_square(self):
        for image, size in self._run_test('force_square'):
            self.assertEqual(image.shape[0], size)
            self.assertEqual(image.shape[1], size)

if __name__ == '__main__':
    unittest.main()
