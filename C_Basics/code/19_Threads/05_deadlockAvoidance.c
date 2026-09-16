#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lockA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockB = PTHREAD_MUTEX_INITIALIZER;

// Deadlock classically happens when thread 1 locks A then waits for B while
// thread 2 locks B then waits for A - neither can proceed. The fix here is
// the simplest one: always acquire locks in the same global order (A before
// B) from every thread, so the cycle can never form.
void *threadOne(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&lockA);
    printf("threadOne: locked A\n");
    pthread_mutex_lock(&lockB);
    printf("threadOne: locked B\n");

    pthread_mutex_unlock(&lockB);
    pthread_mutex_unlock(&lockA);
    return NULL;
}

void *threadTwo(void *arg)
{
    (void)arg;
    // Same order as threadOne (A then B), not B then A - that reversal is
    // exactly what would create the deadlock cycle.
    pthread_mutex_lock(&lockA);
    printf("threadTwo: locked A\n");
    pthread_mutex_lock(&lockB);
    printf("threadTwo: locked B\n");

    pthread_mutex_unlock(&lockB);
    pthread_mutex_unlock(&lockA);
    return NULL;
}

int main()
{
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, threadOne, NULL);
    pthread_create(&t2, NULL, threadTwo, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("both threads finished without deadlock\n");
    return 0;
}
