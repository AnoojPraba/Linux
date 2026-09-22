#include <stdio.h>
#include <pthread.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t forks[NUM_PHILOSOPHERS];

// The classic deadlock setup: if every philosopher picks up their left fork
// first, then waits for their right fork, and all five do this
// simultaneously, each is holding one fork and waiting on a neighbor who is
// doing the same - a circular wait, the same deadlock shape as
// 49_Threads/05_deadlockAvoidance.c's two-thread AB/BA case, just with five
// participants instead of two.
//
// The fix here is the same idea as that file's: break the circular
// ordering. The highest-numbered philosopher picks up their right fork
// (the lower-numbered neighbor's left fork) *first* instead of following
// the same left-then-right order as everyone else - so the cycle of
// "everyone waits on their neighbor" can never fully close.
void *philosopher(void *arg)
{
    long id = (long)arg;
    int leftFork = id;
    int rightFork = (id + 1) % NUM_PHILOSOPHERS;

    if (id == NUM_PHILOSOPHERS - 1)
    {
        int tmp = leftFork;

        leftFork = rightFork;
        rightFork = tmp;
    }

    pthread_mutex_lock(&forks[leftFork]);
    printf("philosopher %ld: picked up fork %d\n", id, leftFork);

    pthread_mutex_lock(&forks[rightFork]);
    printf("philosopher %ld: picked up fork %d, eating\n", id, rightFork);

    printf("philosopher %ld: done eating, putting down forks\n", id);
    pthread_mutex_unlock(&forks[rightFork]);
    pthread_mutex_unlock(&forks[leftFork]);

    return NULL;
}

int main()
{
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int i;

    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_create(&philosophers[i], NULL, philosopher, (void *)(long)i);
    }
    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_join(philosophers[i], NULL);
    }

    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_mutex_destroy(&forks[i]);
    }

    printf("all philosophers finished without deadlock\n");
    return 0;
}
