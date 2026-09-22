#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4
#define ITERATIONS 100000000
#define CACHE_LINE_SIZE 64

// Unpadded: adjacent counters land in the same 64-byte cache line (four
// 8-byte longs fit in one line). Each thread only ever writes its own
// counter - there is no logical race - but every write still invalidates
// that whole cache line in every other core's cache, forcing it to be
// re-fetched. This is false sharing: contention with no shared data.
typedef struct
{
    long value;
} UnpaddedCounter;

// Padded: each counter is forced onto its own cache line by padding the
// struct out to CACHE_LINE_SIZE bytes, so no two threads' counters can
// ever share a line - eliminating the false sharing entirely, purely
// through memory layout, with no locking or atomics involved.
typedef struct
{
    long value;
    char padding[CACHE_LINE_SIZE - sizeof(long)];
} PaddedCounter;

UnpaddedCounter unpaddedCounters[NUM_THREADS];
PaddedCounter paddedCounters[NUM_THREADS];

void *incrementUnpadded(void *arg)
{
    long id = (long)arg;
    long i;

    for (i = 0; i < ITERATIONS; i++)
    {
        unpaddedCounters[id].value++;
    }
    return NULL;
}

void *incrementPadded(void *arg)
{
    long id = (long)arg;
    long i;

    for (i = 0; i < ITERATIONS; i++)
    {
        paddedCounters[id].value++;
    }
    return NULL;
}

double runBenchmark(void *(*threadFunc)(void *))
{
    pthread_t threads[NUM_THREADS];
    struct timespec start;
    struct timespec end;
    long i;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, threadFunc, (void *)i);
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

// This demo relies on the increment actually happening in memory each
// iteration, which is only guaranteed without optimization (this repo's
// Makefile builds with -Wall only, no -O2/-O3) - an aggressive optimizer
// can prove the final value without touching memory every iteration,
// which would eliminate the false sharing this benchmark exists to show.
int main()
{
    double unpaddedTime;
    double paddedTime;

    printf("sizeof(UnpaddedCounter) = %lu bytes (%d fit per %d-byte cache line)\n",
           (unsigned long)sizeof(UnpaddedCounter),
           CACHE_LINE_SIZE / (int)sizeof(UnpaddedCounter), CACHE_LINE_SIZE);
    printf("sizeof(PaddedCounter) = %lu bytes (exactly one per cache line)\n\n",
           (unsigned long)sizeof(PaddedCounter));

    unpaddedTime = runBenchmark(incrementUnpadded);
    printf("unpadded (false sharing): %.3f seconds\n", unpaddedTime);

    paddedTime = runBenchmark(incrementPadded);
    printf("padded (no false sharing): %.3f seconds\n", paddedTime);

    printf("\nunpadded was %.2fx slower\n", unpaddedTime / paddedTime);

    return 0;
}
