try:
    import unittest2 as unittest
except ImportError:
    import unittest

import json
import glob
import hashlib
import gzip
import os
import tempfile
import subprocess
import msgpack
import base64
import math
from model_blame import blame_components


def has_valgrind():
    try:
        return subprocess.Popen(['valgrind', '--help'], stdout=subprocess.PIPE, stderr=subprocess.PIPE).wait() == 0
    except OSError:
        return False


class PicarusModel(object):

    def __init__(self, model_path):
        import picarus_takeout
        self.model = picarus_takeout.ModelChain(gzip.GzipFile(model_path, 'rb').read())

    def process_binary(self, image_path):
        return self.model.process_binary(open(image_path, 'rb').read())

    def process_hash(self, image_path):
        return hashlib.sha1(str(self.process_binary(image_path))).hexdigest()


class PicarusCommandModel(object):

    def __init__(self, model_path, valgrind=False, verbose=False):
        self.cmd = 'picarus'
        self.model_fp = tempfile.NamedTemporaryFile()
        self.model_fp.write(gzip.GzipFile(model_path, 'rb').read())
        self.model_fp.flush()
        self.valgrind = valgrind
        self.verbose = verbose

    def process_binary(self, image_path):
        output_fp = tempfile.NamedTemporaryFile()
        if self.valgrind:
            p = subprocess.Popen(['valgrind', '--suppressions=valgrind_suppressions.val', self.cmd, self.model_fp.name,
                                  image_path, output_fp.name],
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE)
        else:
            p = subprocess.Popen([self.cmd, self.model_fp.name,
                                  image_path, output_fp.name],
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE)
        stdout, stderr = p.communicate()
        if self.verbose:
            print('\n\nstdout')
            print(stdout)
            print('stderr\n\n')
            print(stderr)
        if self.valgrind:
            if stderr.find('ERROR SUMMARY: 0 errors from 0 contexts') == -1:
                raise ValueError
        return open(output_fp.name, 'rb').read()

    def process_hash(self, image_path):
        return hashlib.sha1(self.process_binary(image_path)).hexdigest()


class Test(unittest.TestCase):

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def almostEqualAny(self, a, b, tol=10**-9):
        if a == b:
            return True
        if not isinstance(b, type(a)):
            return False
        if isinstance(a, dict):
            if set(a.keys()) != set(b.keys()):
                return False
            return all(self.assertAlmostEqualAny(v, b[k]) for k, v in a.items())
        elif isinstance(a, (list, tuple)):
            if len(a) != len(b):
                return False
            return all(self.assertAlmostEqualAny(x, y) for x, y in zip(a, b))
        elif isinstance(a, float):
            return math.fabs(a - b) <= tol
        return False

    def _run(self, picarus_model_class):
        results = {}
        model_path = 'picarus_takeout_models/test_models/'
        image_path = 'picarus_takeout_models/test_images/'
        for x in glob.glob(model_path + 'picarus-*.msgpack.gz'):
            model_results = {}
            model = picarus_model_class(x)
            for y in glob.glob(image_path + '*'):
                model_results[os.path.basename(y)] = base64.b64encode(model.process_binary(y))
            results[os.path.basename(x)] = model_results
        json.dump(results, gzip.GzipFile('test_model_outputs-%s.js.gz' % (picarus_model_class.__name__,), 'w'))
        prev_results = json.load(gzip.GzipFile('picarus_takeout_models/test_models/test_model_outputs.js.gz'))
        num_checked = 0
        failed_models = []
        failed_images = {}
        for x in set(results).intersection(set(prev_results)):
            for y in set(results[x]).intersection(set(prev_results[x])):
                num_checked += 1
                if results[x][y] == prev_results[x][y]:
                    continue
                try:
                    a = msgpack.loads(base64.b64decode(results[x][y]))
                except:
                    raise ValueError('Cant test for partial equality because not msgpack encoded')
                b = msgpack.loads(base64.b64decode(prev_results[x][y]))
                if not self.almostEqualAny(a, b):
                    print('Current(b64msgpack)--------')
                    print(results[x][y])
                    print('Previous(b64msgpack)-------')
                    print(prev_results[x][y])
                    print('Current--------')
                    print(msgpack.loads(base64.b64decode(results[x][y])))
                    print('Previous-------')
                    print(msgpack.loads(base64.b64decode(prev_results[x][y])))
                    try:
                        failed_images[y] += 1
                    except KeyError:
                        failed_images[y] = 1
                    failed_models.append(model_path + x)
                    print('Process Failed[%s][%s][%s][%s]' % (x, y, hashlib.sha1(results[x][y]).hexdigest(), hashlib.sha1(prev_results[x][y]).hexdigest()))
        print('Number of models * images checked[%d][%r]' % (num_checked, picarus_model_class))
        blame_components(failed_models)
        print(failed_images)
        self.assertEqual(len(failed_models), 0)

    @unittest.skipUnless(has_valgrind(), 'requires Valgrind')
    def test_valgrind(self):
        for x in glob.glob('picarus_takeout_models/test_models/picarus-*.msgpack.gz'):
            m = PicarusCommandModel(x, valgrind=True)
            for y in glob.glob('picarus_takeout_models/test_images/*'):
                m.process_binary(y)

    def test_compare(self):
        for x in glob.glob('picarus_takeout_models/test_models/picarus-*.msgpack.gz'):
            print(x)
            m0 = PicarusModel(x)
            m1 = PicarusCommandModel(x)
            for y in glob.glob('picarus_takeout_models/test_images/*'):
                outm0 = m0.process_binary(y)
                outm1 = m1.process_binary(y)
                if outm0 != outm1:
                    print(msgpack.loads(outm0))
                    print(msgpack.loads(outm1))
                self.assertEqual(outm0, outm1)

    def test_python(self):
        self._run(PicarusModel)

    def test_cmd(self):
        self._run(PicarusCommandModel)


if __name__ == '__main__':
    unittest.main()
