#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3

void *printThreadId(void *arg)
{
    long id = (long)arg;

    printf("thread %ld running\n", id);
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int i;

    for (i = 0; i < NUM_THREADS; i++)
    {
        // Unlike fork(), threads share the same address space - there is no
        // copy-on-write here, all threads see the same globals and heap.
        pthread_create(&threads[i], NULL, printThreadId, (void *)(long)i);
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        // join() blocks until that thread finishes, same role as waitpid()
        // plays for a child process.
        pthread_join(threads[i], NULL);
    }

    printf("all threads finished\n");
    return 0;
}
