import argparse
import glob
import gzip
import msgpack


def blame_components(models):
    components = {}
    for model_fn in models:
        for x in msgpack.loads(gzip.GzipFile(model_fn).read()):
            try:
                components[x['name']] += 1
            except KeyError:
                components[x['name']] = 1
    print(components)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('model', nargs='+')
    parser.add_argument('--path', default='picarus_takeout_models/test_models/')
    args = parser.parse_args()
    models = []
    print(args.model)
    for model_fn in glob.glob(args.path + '/*'):
        if any(model_fn.find(x) != -1 for x in args.model):
            models.append(model_fn)
    blame_components(models)

if __name__ == '__main__':
    main()
