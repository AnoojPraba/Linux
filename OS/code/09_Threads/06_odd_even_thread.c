#include <stdio.h>
#include <pthread.h>

#define MAX_COUNT 10

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int counter = 1;

void* print_odd(void* arg) {
    while (counter <= MAX_COUNT) {
        pthread_mutex_lock(&lock);

        // Wait while it's an even number's turn
        while (counter % 2 == 0 && counter <= MAX_COUNT) {
            pthread_cond_wait(&cond, &lock);
        }

        if (counter <= MAX_COUNT) {
            printf("Odd Thread  : %d\n", counter++);
            pthread_cond_signal(&cond); // Signal the even thread
        }

        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* print_even(void* arg) {
    while (counter <= MAX_COUNT) {
        pthread_mutex_lock(&lock);

        // Wait while it's an odd number's turn
        while (counter % 2 != 0 && counter <= MAX_COUNT) {
            pthread_cond_wait(&cond, &lock);
        }

        if (counter <= MAX_COUNT) {
            printf("Even Thread : %d\n", counter++);
            pthread_cond_signal(&cond); // Signal the odd thread
        }

        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t_odd, t_even;

    // Create threads
    pthread_create(&t_odd, NULL, print_odd, NULL);
    pthread_create(&t_even, NULL, print_even, NULL);

    // Wait for threads to finish
    pthread_join(t_odd, NULL);
    pthread_join(t_even, NULL);

    // Clean up synchronization primitives
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
