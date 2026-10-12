// Binary search on the answer: integer square root, bisection root finding,
// minimum eating speed and the minimum ship capacity.

#include <math.h>
#include <stdio.h>

static long isqrt(long x)
{
    long lo = 0, hi = x < 2 ? x : x / 2 + 1, ans = 0;
    while (lo <= hi)
    {
        long mid = lo + (hi - lo) / 2;
        if (mid <= x / mid) { ans = mid; lo = mid + 1; }  /* mid*mid <= x without overflow */
        else hi = mid - 1;
    }
    return ans;
}

static double bisect(double (*f)(double), double lo, double hi)
{
    for (int i = 0; i < 60; i++)                          /* each step halves the interval that contains the sign change */
    {
        double mid = (lo + hi) / 2;
        if ((f(lo) < 0) == (f(mid) < 0)) lo = mid; else hi = mid;
    }
    return (lo + hi) / 2;
}

static double cubic(double x) { return x * x * x - x - 2; }

static int hours_needed(const int *piles, int n, int speed)
{
    int h = 0;
    for (int i = 0; i < n; i++) h += (piles[i] + speed - 1) / speed;
    return h;
}

static int days_needed(const int *w, int n, int cap)
{
    int days = 1, load = 0;
    for (int i = 0; i < n; i++)
    {
        if (load + w[i] > cap) { days++; load = 0; }
        load += w[i];
    }
    return days;
}

int main(void)
{
    printf("isqrt(99) = %ld, isqrt(100) = %ld, isqrt(2147395600) = %ld\n", isqrt(99), isqrt(100), isqrt(2147395600L));
    printf("root of x^3 - x - 2 in [1, 2]: %.6f\n", bisect(cubic, 1, 2));

    int piles[] = {3, 6, 7, 11};                          /* the smallest speed so that all piles are eaten within 8 hours */
    int lo = 1, hi = 11;
    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (hours_needed(piles, 4, mid) <= 8) hi = mid; else lo = mid + 1;
    }
    printf("minimum eating speed for 8 hours: %d\n", lo);

    int w[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};            /* the smallest capacity that ships everything in order within 5 days */
    lo = 10; hi = 55;
    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (days_needed(w, 10, mid) <= 5) hi = mid; else lo = mid + 1;
    }
    printf("minimum ship capacity for 5 days: %d\n", lo);
    return 0;
}
