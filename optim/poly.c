#include "poly.h"
#include <time.h>
#include <math.h>

void poly_optim(const double a[], double x, long degree, double *result) 
{
    long i;
    double r[8] = {a[degree], a[degree - 1], a[degree - 2], a[degree - 3], a[degree - 4], a[degree - 5], a[degree - 6], a[degree - 7]};
    double base = ((x*x)*(x*x))*((x*x)*(x*x));

    for (i = degree - 8; i >= 7; i -= 8) 
    {
        r[0] = a[i] + r[0] * base;
        r[1] = a[i - 1] + r[1] * base;
        r[2] = a[i - 2] + r[2] * base;
        r[3] = a[i - 3] + r[3] * base;
        r[4] = a[i - 4] + r[4] * base;
        r[5] = a[i - 5] + r[5] * base;
        r[6] = a[i - 6] + r[6] * base;
        r[7] = a[i - 7] + r[7] * base;
    }
    int n = i + 1;
    for(;n<=7;n++)
    {
        int q = i + 8 - n;
        for(;q>0;q--)
        {
            r[n] *= x;
        }
    }
    for (int j=0; i >= 0; i--,j++) 
    {
        r[j] = (a[i] + r[j] * base);
        for(int m=0;m<i;m++)
        {
            r[j] *= x;
        }
    }


    *result = ((r[0] + r[1]) + (r[2] + r[3])) + ((r[4] + r[5]) + (r[6] + r[7]));
    return;
}

void measure_time(poly_func_t poly, const double a[], double x, long degree,
                  double *time) 
{
    // your code here
    double result;
    struct timespec t1;
    struct timespec t2;
    for(int i=0;i<1000;i++)
    {
        poly(a, x, degree, &result);
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    poly(a, x, degree, &result);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    *time = t2.tv_nsec - t1.tv_nsec;
    return;
}