#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_ITERATIONS 200000
#define THREAD_ZERO 0
#define THREAD_ONE 1

// Dekker's algorithm - the original (1960s) two-process software mutual
// exclusion solution, predating and more complex than Peterson's. Instead
// of a single turn tie-break, a thread that sees the other also wants in
// repeatedly checks whose turn it is; if it isn't its turn, it politely
// backs off (withdraws its own flag, waits, then re-declares interest)
// until turn switches back to it. Peterson's algorithm (01) achieves the
// same guarantees with far less code by combining flag and turn checks
// into one busy-wait condition.
atomic_int wantsToEnter[2] = { 0, 0 };
atomic_int turn = THREAD_ZERO;
int sharedCounter = 0;

/*****************************************************************************
 * Name: enterCriticalSection
 *
 * Description:
 *         Dekker's entry protocol for thread selfId. Declares interest,
 *         then while the other thread also wants in, yields the turn back
 *         and forth until it becomes selfId's turn again.
 *
 * Inputs:
 *         selfId  : the calling thread's id (0 or 1).
 *         otherId : the other thread's id (1 or 0).
 *
 * Returns:
 *         None.
 *****************************************************************************/
void enterCriticalSection(int selfId, int otherId)
{
    atomic_store(&wantsToEnter[selfId], 1);
    while (atomic_load(&wantsToEnter[otherId]))
    {
        if (atomic_load(&turn) != selfId)
        {
            atomic_store(&wantsToEnter[selfId], 0);
            while (atomic_load(&turn) != selfId)
            {
                // busy-wait for our turn
            }
            atomic_store(&wantsToEnter[selfId], 1);
        }
    }
}

/*****************************************************************************
 * Name: exitCriticalSection
 *
 * Description:
 *         Dekker's exit protocol - hands the turn to the other thread and
 *         withdraws the calling thread's interest.
 *
 * Inputs:
 *         selfId  : the calling thread's id (0 or 1).
 *         otherId : the other thread's id (1 or 0).
 *
 * Returns:
 *         None.
 *****************************************************************************/
void exitCriticalSection(int selfId, int otherId)
{
    atomic_store(&turn, otherId);
    atomic_store(&wantsToEnter[selfId], 0);
}

/*****************************************************************************
 * Name: threadBody
 *
 * Description:
 *         Repeatedly enters and exits the Dekker-protected critical
 *         section, incrementing sharedCounter inside it.
 *
 * Inputs:
 *         arg : the thread's id (0 or 1), passed as a pointer-sized value.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *threadBody(void *arg)
{
    int selfId = (int)(long)arg;
    int otherId = (selfId == THREAD_ZERO) ? THREAD_ONE : THREAD_ZERO;
    int i;

    for (i = 0; i < NUM_ITERATIONS; i++)
    {
        enterCriticalSection(selfId, otherId);
        sharedCounter++;
        exitCriticalSection(selfId, otherId);
    }
    return NULL;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Spawns two threads that use Dekker's algorithm to protect a
 *         shared counter increment, then verifies the final count matches
 *         the expected total.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    pthread_t t0;
    pthread_t t1;
    int expected = 2 * NUM_ITERATIONS;

    pthread_create(&t0, NULL, threadBody, (void *)(long)THREAD_ZERO);
    pthread_create(&t1, NULL, threadBody, (void *)(long)THREAD_ONE);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    printf("sharedCounter = %d (expected %d)\n", sharedCounter, expected);

    return 0;
}
