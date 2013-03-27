#ifndef BINARY_COLLECTOR
#define BINARY_COLLECTOR
namespace Picarus {
class BinaryCollector {
public:
    BinaryCollector();
    virtual ~BinaryCollector();
    virtual void operator()(const unsigned char *output, int size) = 0;
};

class CopyCollector: public BinaryCollector {
private:
    unsigned char **output;
    int *size;
public:
    CopyCollector(unsigned char **output, int *size);
    virtual ~CopyCollector();
    virtual void operator()(const unsigned char *output, int size);
};

} // namespace Picarus
#endif
