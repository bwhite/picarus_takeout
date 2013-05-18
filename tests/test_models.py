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


class PicarusModel(object):

    def __init__(self, model_path):
        import picarus_takeout
        self.model = picarus_takeout.ModelChain(gzip.GzipFile(model_path, 'rb').read())

    def process_binary(self, image_path):
        return self.model.process_binary(open(image_path, 'rb').read())

    def process_hash(self, image_path):
        return hashlib.sha1(self.process_binary(image_path)).hexdigest()


class PicarusCommandModel(object):

    def __init__(self, model_path, valgrind=False):
        self.cmd = '../picarus'
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

    def run_tests(self, picarus_model_class):
        results = {}
        for x in glob.glob('picarus_takeout_models/test_models/picarus-*.msgpack.gz'):
            model_results = {}
            model = picarus_model_class(x)
            for y in glob.glob('picarus_takeout_models/test_images/*'):
                model_results[os.path.basename(y)] = model.process_hash(y)
            results[os.path.basename(x)] = model_results
        json.dump(results, open('test_model_outputs.js', 'w'))
        prev_results = json.load(open('picarus_takeout_models/test_models/test_model_outputs.js'))
        num_checked = 0
        for x in set(results).intersection(set(prev_results)):
            for y in set(results[x]).intersection(set(prev_results[x])):
                num_checked += 1
                if results[x][y] != prev_results[x][y]:
                    print('Process Failed[%s][%s][%s][%s]' % (x, y, results[x][y], prev_results[x][y]))
                self.assertEqual(results[x][y], prev_results[x][y])
        print('Number of models * images checked[%d]' % num_checked)

    def test_compare(self):
        model_hash = 'c880581bd1740b1148bae61e5b06d90309acaeab'
        for x in glob.glob('picarus_takeout_models/test_models/picarus-*.msgpack.gz'):
            if x.find(model_hash) == -1:
                continue
            m0 = PicarusModel(x)
            m1 = PicarusCommandModel(x)
            for y in glob.glob('picarus_takeout_models/test_images/*'):
                self.assertEqual(m0.process_hash(y), m1.process_hash(y))

    def test_valgrind(self):
        for x in glob.glob('picarus_takeout_models/test_models/picarus-*.msgpack.gz'):
            m = PicarusCommandModel(x, valgrind=True)
            for y in glob.glob('picarus_takeout_models/test_images/*'):
                m.process_binary(y)

    def test_python(self):
        self.run_tests(PicarusModel)

    def test_cmd(self):
        self.run_tests(PicarusCommandModel)


if __name__ == '__main__':
    unittest.main()
