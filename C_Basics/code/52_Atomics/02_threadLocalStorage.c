#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3

// _Thread_local (or the pre-C11 GNU/MSVC extension __thread) gives each
// thread its own independent copy of this variable - unlike an ordinary
// global, which every thread shares and would need a mutex to touch
// safely (see 49_Threads/02_mutex.c). No locking is needed here because
// there's nothing shared to race on.
_Thread_local int threadCallCount = 0;

void *worker(void *arg)
{
    long id = (long)arg;
    int i;

    for (i = 0; i < 3; i++)
    {
        threadCallCount++;
        printf("thread %ld: threadCallCount = %d\n", id, threadCallCount);
    }
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int i;

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, worker, (void *)(long)i);
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
