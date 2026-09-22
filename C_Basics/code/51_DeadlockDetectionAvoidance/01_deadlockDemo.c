#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define ACQUIRE_DELAY_USEC 100000

// Two threads acquire the same two mutexes in opposite order - classic
// circular-wait setup. This program deliberately deadlocks and will hang;
// it exists to be read, not necessarily run to completion (kill it after
// observing the "acquired first lock" prints with no further progress).
pthread_mutex_t mutexA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexB = PTHREAD_MUTEX_INITIALIZER;

void *threadTakesAThenB(void *arg)
{
    (void)arg;

    pthread_mutex_lock(&mutexA);
    printf("thread A->B: acquired mutexA\n");
    usleep(ACQUIRE_DELAY_USEC);

    pthread_mutex_lock(&mutexB);
    printf("thread A->B: acquired mutexB\n");

    pthread_mutex_unlock(&mutexB);
    pthread_mutex_unlock(&mutexA);
    return NULL;
}

void *threadTakesBThenA(void *arg)
{
    (void)arg;

    pthread_mutex_lock(&mutexB);
    printf("thread B->A: acquired mutexB\n");
    usleep(ACQUIRE_DELAY_USEC);

    // Deadlock: this thread waits for mutexA (held by the other thread,
    // which is waiting for mutexB, held by this thread) - circular wait,
    // the fourth Coffman condition alongside mutual exclusion, hold-and-
    // wait, and no preemption.
    pthread_mutex_lock(&mutexA);
    printf("thread B->A: acquired mutexA\n");

    pthread_mutex_unlock(&mutexA);
    pthread_mutex_unlock(&mutexB);
    return NULL;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Spawns two threads that acquire mutexA/mutexB in opposite order,
 *         demonstrating the circular-wait condition that causes deadlock.
 *         This program will hang - see 02_deadlockFixed.c for the fix.
 *
 * Returns:
 *         0 on success (never reached in practice).
 *****************************************************************************/
int main()
{
    pthread_t t1;
    pthread_t t2;

    printf("this demo deliberately deadlocks - interrupt with Ctrl+C\n");

    pthread_create(&t1, NULL, threadTakesAThenB, NULL);
    pthread_create(&t2, NULL, threadTakesBThenA, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("both threads finished (deadlock did not occur this run)\n");
    return 0;
}
