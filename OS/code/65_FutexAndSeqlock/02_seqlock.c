#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

// Seqlock: single writer, many readers, readers never block the writer.
// A sequence counter is odd while a write is in progress; a reader retries if
// it saw an odd value or the counter changed during its read. Good for small,
// frequently read, rarely written data (the kernel uses it for jiffies/time).
// Not suitable for data containing pointers the reader dereferences.
//
// The payload fields are relaxed atomics: a plain read racing with the writer
// would be a data race (UB) in the C11 model even though the retry discards
// the torn result.
struct seqlock
{
    atomic_uint seq;
    atomic_long a, b;     // invariant: b == 2 * a
};

static struct seqlock sl;

static void write_pair(long v)
{
    unsigned s = atomic_load_explicit(&sl.seq, memory_order_relaxed);
    atomic_store_explicit(&sl.seq, s + 1, memory_order_relaxed);   // -> odd
    atomic_thread_fence(memory_order_release);
    atomic_store_explicit(&sl.a, v, memory_order_relaxed);
    atomic_store_explicit(&sl.b, 2 * v, memory_order_relaxed);
    atomic_store_explicit(&sl.seq, s + 2, memory_order_release);   // -> even
}

static void read_pair(long *a, long *b, unsigned long *retries)
{
    unsigned s1, s2;
    do
    {
        s1 = atomic_load_explicit(&sl.seq, memory_order_acquire);
        *a = atomic_load_explicit(&sl.a, memory_order_relaxed);
        *b = atomic_load_explicit(&sl.b, memory_order_relaxed);
        atomic_thread_fence(memory_order_acquire);
        s2 = atomic_load_explicit(&sl.seq, memory_order_relaxed);
        if ((s1 & 1) || s1 != s2)
            (*retries)++;
    } while ((s1 & 1) || s1 != s2);
}

static void *writer(void *arg)
{
    (void)arg;
    for (long v = 1; v <= 2000000; v++)
        write_pair(v);
    return NULL;
}

static void *reader(void *arg)
{
    unsigned long retries = 0, torn = 0, reads = 0;
    for (int i = 0; i < 2000000; i++)
    {
        long a, b;
        read_pair(&a, &b, &retries);
        reads++;
        if (b != 2 * a)
            torn++;
    }
    printf("reader %ld: reads=%lu retries=%lu torn=%lu\n", (long)arg, reads,
           retries, torn);
    return NULL;
}

int main(void)
{
    pthread_t w, r[3];
    pthread_create(&w, NULL, writer, NULL);
    for (long i = 0; i < 3; i++)
        pthread_create(&r[i], NULL, reader, (void *)i);
    pthread_join(w, NULL);
    for (int i = 0; i < 3; i++)
        pthread_join(r[i], NULL);
    return 0;
}
