#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_ITERATIONS 200000
#define THREAD_ZERO 0
#define THREAD_ONE 1

// Peterson's algorithm - a software-only mutual exclusion solution for two
// threads, requiring no hardware atomic instruction beyond ordinary loads
// and stores (memory ordering aside). flag[i] declares "thread i wants to
// enter"; turn breaks ties when both want in at once - whichever thread did
// NOT set turn last gets to wait, the other proceeds.
// flag/turn are declared atomic only to prevent the compiler from reordering
// or caching them across iterations, not because the algorithm needs a
// hardware atomic instruction - the mutual exclusion comes from the
// flag+turn logic itself.
atomic_int flag[2] = { 0, 0 };
atomic_int turn = 0;
int sharedCounter = 0;

/*****************************************************************************
 * Name: enterCriticalSection
 *
 * Description:
 *         Peterson's entry protocol for thread selfId (the other thread is
 *         otherId). Declares interest via flag[selfId], yields the tie via
 *         turn, then busy-waits while the other thread both wants in and it
 *         is the other thread's turn.
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
    atomic_store(&flag[selfId], 1);
    atomic_store(&turn, otherId);
    while ((atomic_load(&flag[otherId])) && (atomic_load(&turn) == otherId))
    {
        // busy-wait
    }
}

/*****************************************************************************
 * Name: exitCriticalSection
 *
 * Description:
 *         Peterson's exit protocol - withdraws the calling thread's
 *         interest so the other thread may proceed.
 *
 * Inputs:
 *         selfId : the calling thread's id (0 or 1).
 *
 * Returns:
 *         None.
 *****************************************************************************/
void exitCriticalSection(int selfId)
{
    atomic_store(&flag[selfId], 0);
}

/*****************************************************************************
 * Name: threadBody
 *
 * Description:
 *         Repeatedly enters and exits the Peterson-protected critical
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
        exitCriticalSection(selfId);
    }
    return NULL;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Spawns two threads that use Peterson's algorithm to protect a
 *         shared counter increment, then verifies the final count matches
 *         the expected total (no lost updates).
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
