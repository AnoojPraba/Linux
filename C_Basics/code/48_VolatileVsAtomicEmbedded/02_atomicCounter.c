#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ITERATIONS 100000

// _Atomic gives both atomicity (no lost updates from a torn read-modify-
// write) and a defined memory ordering across threads - what real
// concurrent access needs, unlike plain volatile.
atomic_int sharedCounter = 0;

void *incrementAtomic(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < ITERATIONS; i++)
    {
        atomic_fetch_add(&sharedCounter, 1);
    }
    return NULL;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Runs multiple threads incrementing a C11 _Atomic counter and
 *         confirms the final value has no lost updates, contrasting with
 *         a plain volatile int which would lose updates under contention.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    pthread_t threads[NUM_THREADS];
    int i;

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, incrementAtomic, NULL);
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("final counter = %d (expected %d)\n", atomic_load(&sharedCounter),
           NUM_THREADS * ITERATIONS);

    return 0;
}
