#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTS_PER_THREAD 200000

// counter++ is not atomic - it decomposes into a read, a modify, and a
// write. When two threads interleave those three steps on the same shared
// counter, one thread's update can be silently lost (a "lost update").
// This is a genuine race condition: the final result depends on the
// timing/interleaving of the threads, not just their logic.
int counter = 0;

/*****************************************************************************
 * Name: incrementCounter
 *
 * Description:
 *         Increments the shared, unprotected counter INCREMENTS_PER_THREAD
 *         times. The increment is a read-modify-write critical section with
 *         no mutual exclusion, so concurrent runs race on counter.
 *
 * Inputs:
 *         arg : unused.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
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

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Spawns NUM_THREADS threads that each increment a shared,
 *         unprotected counter, then prints the final count next to the
 *         expected count. The two almost never match, demonstrating lost
 *         updates from an unprotected critical section.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
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
        printf("lost updates detected - this is the race condition in action\n");
    }

    return 0;
}
