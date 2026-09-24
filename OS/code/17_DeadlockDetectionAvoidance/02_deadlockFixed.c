#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define ACQUIRE_DELAY_USEC 100000

pthread_mutex_t mutexA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexB = PTHREAD_MUTEX_INITIALIZER;

// Fix #1: consistent lock ordering. Both threads always acquire mutexA
// before mutexB, which eliminates circular wait entirely - one of the four
// necessary conditions for deadlock is now impossible.
void *threadOrderedAThenB(void *arg)
{
    (void)arg;

    pthread_mutex_lock(&mutexA);
    printf("ordered thread: acquired mutexA\n");
    usleep(ACQUIRE_DELAY_USEC);

    pthread_mutex_lock(&mutexB);
    printf("ordered thread: acquired mutexB\n");

    pthread_mutex_unlock(&mutexB);
    pthread_mutex_unlock(&mutexA);
    return NULL;
}

void *threadOrderedAThenB2(void *arg)
{
    (void)arg;

    pthread_mutex_lock(&mutexA);
    printf("ordered thread 2: acquired mutexA\n");
    usleep(ACQUIRE_DELAY_USEC);

    pthread_mutex_lock(&mutexB);
    printf("ordered thread 2: acquired mutexB\n");

    pthread_mutex_unlock(&mutexB);
    pthread_mutex_unlock(&mutexA);
    return NULL;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Runs two threads that both acquire mutexA before mutexB (fix #1:
 *         consistent lock ordering), demonstrating deadlock avoidance
 *         without needing trylock/backoff.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, threadOrderedAThenB, NULL);
    pthread_create(&t2, NULL, threadOrderedAThenB2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("both threads finished cleanly - no deadlock\n");
    return 0;
}
