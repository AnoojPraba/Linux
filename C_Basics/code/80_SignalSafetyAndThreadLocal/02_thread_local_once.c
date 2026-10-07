#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// _Thread_local (C11) / __thread (GNU): each thread gets its own copy.
// errno is the canonical example - it's a per-thread lvalue, not a global.
static _Thread_local int tls_counter = 0;
static int shared_counter = 0;          // shared: guarded by `lock`

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// pthread_once: run an initializer exactly once even if many threads race to
// call it (C11 equivalent: call_once in <threads.h>, not in glibc <2.28).
static pthread_once_t once = PTHREAD_ONCE_INIT;
static int table[4];

static void init_table(void)
{
    puts("init_table runs once");
    for (int i = 0; i < 4; i++)
        table[i] = i * i;
}

static void *worker(void *arg)
{
    long id = (long)arg;
    pthread_once(&once, init_table);
    for (int i = 0; i < 1000; i++)
    {
        tls_counter++;                       // no lock needed: private
        pthread_mutex_lock(&lock);
        shared_counter++;                    // shared: needs the mutex
        pthread_mutex_unlock(&lock);
    }
    printf("thread %ld: tls_counter=%d (&tls=%p) table[3]=%d\n", id, tls_counter,
           (void *)&tls_counter, table[3]);
    return NULL;
}

int main(void)
{
    pthread_t t[3];
    for (long i = 0; i < 3; i++)
        pthread_create(&t[i], NULL, worker, (void *)i);
    for (int i = 0; i < 3; i++)
        pthread_join(t[i], NULL);
    printf("shared_counter=%d main tls_counter=%d\n", shared_counter, tls_counter);
    return 0;
}
