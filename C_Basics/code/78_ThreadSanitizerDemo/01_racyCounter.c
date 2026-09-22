#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTS_PER_THREAD 100000

// Deliberately unsynchronized - the same mistake 49_Threads/02_mutex.c
// fixes with a mutex and 52_Atomics/01_stdatomicCounter.c fixes with an
// atomic. Left broken on purpose here, specifically so ThreadSanitizer
// has a real, reliable race to detect.
int counter = 0;

void *incrementCounter(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < INCREMENTS_PER_THREAD; i++)
    {
        counter++;
    }
    return NULL;
}

// Compile and run this file with: gcc -fsanitize=thread -pthread
// 01_racyCounter.c -o racy && ./racy - ThreadSanitizer instruments every
// memory access and reports the exact two conflicting accesses (which
// threads, which source lines) the moment it observes an unsynchronized
// read/write race, rather than waiting for the corruption to eventually
// produce a visibly wrong final count the way this program's own printed
// total does (and even that only shows up unreliably, since races don't
// always lose an increment on every run).
int main()
{
    pthread_t threads[NUM_THREADS];
    int i;
    int expected = NUM_THREADS * INCREMENTS_PER_THREAD;

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, incrementCounter, NULL);
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("counter = %d (expected %d)\n", counter, expected);
    if (counter != expected)
    {
        printf("lost updates due to the unsynchronized race, as expected\n");
    }

    return 0;
}
