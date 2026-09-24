#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4

pthread_barrier_t barrier;

/*****************************************************************************
 * Name: phaseWorker
 *
 * Description:
 *         Simulates a phased parallel computation: does some "phase 1" work
 *         then waits at the barrier for every other thread to finish phase 1
 *         before any thread is allowed to start phase 2.
 *
 * Inputs:
 *         arg : worker id, passed as a pointer-sized integer.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *phaseWorker(void *arg)
{
    long id = (long)arg;

    // Stagger arrival so the barrier's rendezvous behavior is visible - id 0
    // sleeps longest, so the others should print "waiting" before it arrives.
    sleep((unsigned int)id);
    printf("thread %ld: finished phase 1, waiting at barrier\n", id);

    // Blocks until NUM_THREADS threads have all called pthread_barrier_wait -
    // only then does any of them proceed. Exactly one caller gets the return
    // value PTHREAD_BARRIER_SERIAL_THREAD; the rest get 0 - useful if one
    // thread needs to do a single piece of cleanup/setup between phases.
    pthread_barrier_wait(&barrier);

    printf("thread %ld: starting phase 2\n", id);
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int i;

    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, phaseWorker, (void *)(long)i);
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    return 0;
}
