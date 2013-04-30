#ifndef PICARUS_MATH
#define PICARUS_MATH

#ifdef __cplusplus 
extern "C" {
#endif

    double dot_product(double *v0, double *v1, int size);
#ifdef WIN32
    double round(double r);
    double log2(double n);
#endif

#ifdef __cplusplus 
}
#endif

// From http://tdistler.com/2011/03/24/how-to-define-nan-not-a-number-on-windows
#ifdef WIN32
#ifndef NAN
static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
#define NAN (*(const float *) __nan)
#endif // NAN
#ifndef INFINITY 
static const unsigned __int8 __infinity[4] = {0x00, 0x00, 0x80, 0x7F};
#define INFINITY (*(const float *) __infinity)
#endif // INFINITY
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI
#endif // WIN32
#endif
