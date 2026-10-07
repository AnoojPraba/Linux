#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define N (1 << 22)

static double now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static int cmp(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

// Sum values >= 128. With random data the branch is unpredictable; sorted
// data makes it almost perfectly predictable. Same work, different speed.
// volatile sink + noinline keep the compiler from vectorizing or turning the
// branch into a conditional move (which would hide the effect).
__attribute__((noinline)) static long sum_branchy(const int *v, int n)
{
    long s = 0;
    for (int i = 0; i < n; i++)
        if (v[i] >= 128)
            s += v[i];
    return s;
}

// Branchless form: mask is all-ones when v >= 128, else 0.
__attribute__((noinline)) static long sum_branchless(const int *v, int n)
{
    long s = 0;
    for (int i = 0; i < n; i++)
    {
        int mask = -(v[i] >= 128);
        s += v[i] & mask;
    }
    return s;
}

static int checked_div(int a, int b)
{
    if (unlikely(b == 0))       // cold error path moved out of the hot line
    {
        fprintf(stderr, "div by zero\n");
        return 0;
    }
    return a / b;
}

int main(void)
{
    int *v = malloc(N * sizeof *v);
    if (!v)
        return 1;
    srand(1);
    for (int i = 0; i < N; i++)
        v[i] = rand() % 256;

    double t = now();
    long a = sum_branchy(v, N);
    printf("random  branchy   : %ld  %.4fs\n", a, now() - t);

    t = now();
    a = sum_branchless(v, N);
    printf("random  branchless: %ld  %.4fs\n", a, now() - t);

    qsort(v, N, sizeof *v, cmp);

    t = now();
    a = sum_branchy(v, N);
    printf("sorted  branchy   : %ld  %.4fs\n", a, now() - t);

    printf("checked_div(10,2)=%d\n", checked_div(10, 2));
    free(v);
    return 0;
}
