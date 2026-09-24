#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 4000

// Lamport's Bakery algorithm - generalizes Peterson's/Dekker's two-thread
// solutions to N threads, modeled on a bakery's take-a-number system. Each
// thread picks a ticket number one greater than the max of all currently
// held tickets, then waits until no other thread with a "smaller" ticket
// (ties broken by thread id) still wants to enter. No thread can be
// starved: any thread waiting for the critical section eventually has the
// lowest active ticket, since new arrivals always get strictly higher
// numbers than everyone already waiting.
atomic_int choosing[NUM_THREADS];
atomic_int ticket[NUM_THREADS];
int sharedCounter = 0;

/*****************************************************************************
 * Name: ticketIsLower
 *
 * Description:
 *         Compares two threads' (ticket, id) pairs using bakery ordering -
 *         lower ticket wins, ties broken by lower thread id.
 *
 * Inputs:
 *         ticketA : ticket number of thread A.
 *         idA     : id of thread A.
 *         ticketB : ticket number of thread B.
 *         idB     : id of thread B.
 *
 * Returns:
 *         Nonzero if (ticketA, idA) is strictly lower priority order than
 *         (ticketB, idB), zero otherwise.
 *****************************************************************************/
int ticketIsLower(int ticketA, int idA, int ticketB, int idB)
{
    if (ticketA != ticketB)
    {
        return (ticketA < ticketB);
    }
    return (idA < idB);
}

/*****************************************************************************
 * Name: enterCriticalSection
 *
 * Description:
 *         Bakery entry protocol for thread selfId - takes a ticket one
 *         greater than the current max, then waits for every other thread
 *         with a lower (ticket, id) pair to finish its critical section.
 *
 * Inputs:
 *         selfId : the calling thread's id.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void enterCriticalSection(int selfId)
{
    int i;
    int maxTicket = 0;

    atomic_store(&choosing[selfId], 1);
    for (i = 0; i < NUM_THREADS; i++)
    {
        int otherTicket = atomic_load(&ticket[i]);

        if (otherTicket > maxTicket)
        {
            maxTicket = otherTicket;
        }
    }
    atomic_store(&ticket[selfId], maxTicket + 1);
    atomic_store(&choosing[selfId], 0);

    for (i = 0; i < NUM_THREADS; i++)
    {
        if (i == selfId)
        {
            continue;
        }
        while (atomic_load(&choosing[i]))
        {
            // busy-wait for thread i to finish choosing its ticket
        }
        while ((atomic_load(&ticket[i]) != 0) &&
               (ticketIsLower(atomic_load(&ticket[i]), i, atomic_load(&ticket[selfId]),
                               selfId)))
        {
            // busy-wait while thread i has strictly higher priority
        }
    }
}

/*****************************************************************************
 * Name: exitCriticalSection
 *
 * Description:
 *         Bakery exit protocol - releases the calling thread's ticket back
 *         to zero (not holding a ticket).
 *
 * Inputs:
 *         selfId : the calling thread's id.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void exitCriticalSection(int selfId)
{
    atomic_store(&ticket[selfId], 0);
}

/*****************************************************************************
 * Name: threadBody
 *
 * Description:
 *         Repeatedly enters and exits the bakery-protected critical
 *         section, incrementing sharedCounter inside it.
 *
 * Inputs:
 *         arg : the thread's id, passed as a pointer-sized value.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *threadBody(void *arg)
{
    int selfId = (int)(long)arg;
    int i;

    for (i = 0; i < NUM_ITERATIONS; i++)
    {
        enterCriticalSection(selfId);
        sharedCounter++;
        exitCriticalSection(selfId);
    }
    return NULL;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Spawns NUM_THREADS threads that use Lamport's Bakery algorithm to
 *         protect a shared counter increment, then verifies the final
 *         count matches the expected total.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    pthread_t threads[NUM_THREADS];
    int i;
    int expected = NUM_THREADS * NUM_ITERATIONS;

    for (i = 0; i < NUM_THREADS; i++)
    {
        atomic_store(&choosing[i], 0);
        atomic_store(&ticket[i], 0);
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, threadBody, (void *)(long)i);
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("sharedCounter = %d (expected %d)\n", sharedCounter, expected);

    return 0;
}
