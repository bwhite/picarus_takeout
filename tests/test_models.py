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

    def __init__(self, model_path, valgrind=False):
        self.cmd = 'picarus'
        self.model_fp = tempfile.NamedTemporaryFile()
        self.model_fp.write(gzip.GzipFile(model_path, 'rb').read())
        self.model_fp.flush()
        self.valgrind = valgrind

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

    def _run(self, picarus_model_class):
        results = {}
        model_path = 'picarus_takeout_models/test_models/'
        image_path = 'picarus_takeout_models/test_images/'
        for x in glob.glob(model_path + 'picarus-*.msgpack.gz'):
            model_results = {}
            model = picarus_model_class(x)
            for y in glob.glob(image_path + '*'):
                model_results[os.path.basename(y)] = model.process_hash(y)
            results[os.path.basename(x)] = model_results
        json.dump(results, open('test_model_outputs-%s.js' % (picarus_model_class.__name__,), 'w'))
        prev_results = json.load(open('picarus_takeout_models/test_models/test_model_outputs.js'))
        num_checked = 0
        failed_models = []
        failed_images = {}
        for x in set(results).intersection(set(prev_results)):
            for y in set(results[x]).intersection(set(prev_results[x])):
                num_checked += 1
                if results[x][y] != prev_results[x][y]:
                    # TODO: Cache results?
                    print(msgpack.loads(picarus_model_class(model_path + x).process_binary(image_path + y)))
                    try:
                        failed_images[y] += 1
                    except KeyError:
                        failed_images[y] = 1
                    failed_models.append(model_path + x)
                    print('Process Failed[%s][%s][%s][%s]' % (x, y, results[x][y], prev_results[x][y]))
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
