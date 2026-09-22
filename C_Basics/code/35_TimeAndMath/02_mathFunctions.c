#include <stdio.h>
#include <math.h>

int main()
{
    double x = 2.0;

    printf("sqrt(%.1f) = %f\n", x, sqrt(x));
    printf("pow(%.1f, 10) = %f\n", x, pow(x, 10));
    printf("floor(3.7) = %f\n", floor(3.7));
    printf("ceil(3.2) = %f\n", ceil(3.2));
    printf("fabs(-5.5) = %f\n", fabs(-5.5));

    // Standard trig functions work in radians, not degrees - M_PI (from
    // math.h, a common GNU/POSIX extension to the C standard) is the
    // usual way to convert.
    printf("sin(M_PI / 2) = %f\n", sin(M_PI / 2));

    // NAN and isnan(): some math operations (0.0/0.0, sqrt of a negative
    // number) don't have a real result - they produce NAN instead of
    // crashing, and isnan() is the only correct way to test for it (a NAN
    // compared with == is never equal to anything, even itself).
    printf("sqrt(-1.0) is NaN: %d\n", isnan(sqrt(-1.0)));

    return 0;
}
