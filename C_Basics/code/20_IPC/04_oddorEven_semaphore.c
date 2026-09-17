#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_COUNT 10

// Declare two semaphores
sem_t sem_odd;
sem_t sem_even;

int counter = 1;

void *print_odd(void *arg)
{
    while(counter <= MAX_COUNT)
    {
        sem_wait(&sem_odd); // Wait for permission to print odd number

        if(counter <= MAX_COUNT)
        {
            printf("Odd Thread  : %d\n", counter++);
        }

        sem_post(&sem_even); // Signal the even thread to proceed
    }
    return NULL;
}

void *print_even(void *arg)
{
    while(counter <= MAX_COUNT)
    {
        sem_wait(&sem_even); // Wait for permission to print even number

        if(counter <= MAX_COUNT)
        {
            printf("Even Thread : %d\n", counter++);
        }

        sem_post(&sem_odd); // Signal the odd thread to proceed
    }
    return NULL;
}

int main()
{
    pthread_t t_odd, t_even;

    // Initialize semaphores:
    // sem_odd starts at 1 so the odd thread executes first.
    // sem_even starts at 0 so the even thread waits initially.
    sem_init(&sem_odd, 0, 1);
    sem_init(&sem_even, 0, 0);

    // Create threads
    pthread_create(&t_odd, NULL, print_odd, NULL);
    pthread_create(&t_even, NULL, print_even, NULL);

    // Wait for completion
    pthread_join(t_odd, NULL);
    pthread_join(t_even, NULL);

    // Destroy semaphores
    sem_destroy(&sem_odd);
    sem_destroy(&sem_even);

    return 0;
}
