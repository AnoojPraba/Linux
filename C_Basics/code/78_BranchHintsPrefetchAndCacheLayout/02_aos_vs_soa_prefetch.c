#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N (1 << 22)

static double now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

// Array of Structures: scanning only 'x' still drags every other field
// through the cache - 15 of every 16 loaded 4-byte words are wasted.
struct particle_aos
{
    float x, y, z;
    float vx, vy, vz;
    float mass;
    int id;
    float extra[8];     // rest of the object: one struct = one 64B cache line
};

// Structure of Arrays: each field contiguous. A scan over 'x' uses every byte
// of every cache line (and auto-vectorizes).
struct particles_soa
{
    float *x, *y, *z;
    float *vx, *vy, *vz;
    float *mass;
    int *id;
};

// Linked-list-style indirect access: next index is data dependent, so the
// hardware prefetcher cannot guess it. Software prefetch of a known-future
// element hides latency.
static long gather_sum(const int *idx, const int *table, int n, int prefetch)
{
    long s = 0;
    for (int i = 0; i < n; i++)
    {
        if (prefetch && i + 16 < n)
            __builtin_prefetch(&table[idx[i + 16]], 0 /*read*/, 1 /*low reuse*/);
        s += table[idx[i]];
    }
    return s;
}

int main(void)
{
    struct particle_aos *a = malloc(N * sizeof *a);
    struct particles_soa s = { .x = malloc(N * sizeof(float)) };
    if (!a || !s.x)
        return 1;
    for (int i = 0; i < N; i++)
        a[i].x = s.x[i] = (float)i;

    double t = now();
    float sa = 0;
    for (int i = 0; i < N; i++)
        sa += a[i].x;
    double t_aos = now() - t;

    t = now();
    float ss = 0;
    for (int i = 0; i < N; i++)
        ss += s.x[i];
    double t_soa = now() - t;

    printf("sizeof(particle_aos)=%zu\n", sizeof *a);
    printf("AoS scan x: %.4fs   SoA scan x: %.4fs   (sums %.0f %.0f)\n",
           t_aos, t_soa, sa, ss);

    int *table = malloc(N * sizeof *table);
    int *idx = malloc(N * sizeof *idx);
    if (!table || !idx)
        return 1;
    for (int i = 0; i < N; i++)
    {
        table[i] = i & 0xFF;
        idx[i] = rand() % N;      // random gather = cache-miss heavy
    }

    t = now();
    long g0 = gather_sum(idx, table, N, 0);
    double t0 = now() - t;
    t = now();
    long g1 = gather_sum(idx, table, N, 1);
    double t1 = now() - t;
    printf("random gather: no prefetch %.4fs, prefetch %.4fs (%ld %ld)\n",
           t0, t1, g0, g1);

    free(a); free(s.x); free(table); free(idx);
    return 0;
}
