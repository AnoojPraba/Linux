#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_WORKERS 5
#define MAX_CONCURRENT 2

sem_t slotSemaphore;

void *worker(void *arg)
{
    long id = (long)arg;

    // Unlike a mutex (max 1 holder), a counting semaphore allows up to its
    // initial count of threads through at once - here, at most
    // MAX_CONCURRENT workers run their "critical section" simultaneously.
    sem_wait(&slotSemaphore);
    printf("worker %ld: got a slot\n", id);
    printf("worker %ld: releasing slot\n", id);
    sem_post(&slotSemaphore);

    return NULL;
}

int main()
{
    pthread_t threads[NUM_WORKERS];
    int i;

    // Second arg 0 means "shared between threads of this process only" (not
    // across processes - see the shared, process-visible version in
    // 45_IPC/03_processSemaphore.c).
    sem_init(&slotSemaphore, 0, MAX_CONCURRENT);

    for (i = 0; i < NUM_WORKERS; i++)
    {
        pthread_create(&threads[i], NULL, worker, (void *)(long)i);
    }
    for (i = 0; i < NUM_WORKERS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&slotSemaphore);
    return 0;
}
