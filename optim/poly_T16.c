#include "poly.h"
#include <time.h>

void poly_optim(const double a[], double x, long degree, double *result) 
{
    // your code here
    long i;
    double base1 = x;
    double base2 = x*x;
    double base3 = x*x*x;
    double base4 = base2*base2;
    double base5 = base2*base3; 
    double base6 = base3*base3; 
    double base7 = base3*base4; 
    double base8 = base4*base4; 
    double base9 = base4*base5; 
    double base10 = base5*base5;
    double base11 = base5*base6;
    double base12 = base6*base6;
    double base13 = base6*base7;
    double base14 = base7*base7;
    double base15 = base8*base7;
    double base16 = base8*base8;
    double acc = a[degree]; 
    double acc1;
    double acc2;
    double acc3;
    double acc4;
    double acc5;
    double acc6;
    double acc7;
    double acc8;
    double acc9;
    double acc10;
    double acc11;
    double acc12;
    double acc13;
    double acc14;
    double acc15;
    double acc16;
    for(i=degree;i>=16;i-=16)
    {
        acc1 = acc*base16; 
        acc2 = a[i-1]*base15;
        acc3 = a[i-2]*base14;
        acc4 = a[i-3]*base13;
        acc5 = a[i-4]*base12;
        acc6 = a[i-5]*base11;
        acc7 = a[i-6]*base10;
        acc8 = a[i-7]*base9;
        acc9 = a[i-8]*base8;
        acc10 = a[i-9]*base7;
        acc11 = a[i-10]*base6;
        acc12 = a[i-11]*base5;
        acc13 = a[i-12]*base4;
        acc14 = a[i-13]*base3;
        acc15 = a[i-14]*base2;
        acc16 = a[i-15]*base1;
        acc1 += acc2;
        acc3 += acc4;
        acc5 += acc6;
        acc7 += acc8;
        acc9 += acc10;
        acc11 += acc12;
        acc13 += acc14;
        acc15 += acc16;
        acc1 += acc3;
        acc5 += acc7;
        acc9 += acc11;
        acc13 += acc15;
        acc1 += acc5;
        acc9 += acc13;
        acc = (acc1) + (acc9 + a[i-16]); 
    }
    for(;i>=1;i--)
    {
        acc = acc*x + a[i-1];
    }
    *result = acc;
    return;
}

void measure_time(poly_func_t poly, const double a[], double x, long degree,
                  double *time) 
{
    // your code here
    double result;
    poly(a, x, degree, &result);
    struct timespec t1;
    struct timespec t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    poly(a, x, degree, &result);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    *time = t2.tv_nsec - t1.tv_nsec;
    return;
}