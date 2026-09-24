#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 4
#define NUM_ITERATIONS 200000

// The software-only algorithms in this folder (Peterson's, Dekker's,
// Bakery) were needed before CPUs offered atomic read-modify-write
// instructions. Modern hardware provides test-and-set and
// compare-and-swap (CAS) as single indivisible instructions, so a spinlock
// can be built directly on top of one atomic variable instead of the
// multi-variable flag/turn/ticket bookkeeping those algorithms need.
// This demo builds a spinlock two ways: atomic_flag (hardware test-and-set)
// and an atomic_int with atomic_compare_exchange (hardware CAS).
// Each lock protects its own counter - mixing two independently-acquired
// locks to protect the SAME shared variable would not provide mutual
// exclusion between them (a thread holding one lock wouldn't exclude a
// thread holding the other), so the two lock styles are kept and verified
// separately here.
atomic_flag testAndSetLock = ATOMIC_FLAG_INIT;
atomic_int casLock = 0;
int testAndSetCounter = 0;
int casCounter = 0;

/*****************************************************************************
 * Name: testAndSetAcquire
 *
 * Description:
 *         Spins until testAndSetLock is atomically set, i.e. until this
 *         call is the one that flips it from clear to set.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void testAndSetAcquire(void)
{
    while (atomic_flag_test_and_set(&testAndSetLock))
    {
        // busy-wait: someone else holds the lock
    }
}

/*****************************************************************************
 * Name: testAndSetRelease
 *
 * Description:
 *         Clears testAndSetLock, releasing it for other threads.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void testAndSetRelease(void)
{
    atomic_flag_clear(&testAndSetLock);
}

/*****************************************************************************
 * Name: casAcquire
 *
 * Description:
 *         Spins doing compare-and-swap attempts, trying to atomically flip
 *         casLock from 0 (unlocked) to 1 (locked).
 *
 * Returns:
 *         None.
 *****************************************************************************/
void casAcquire(void)
{
    int expected;

    for (;;)
    {
        expected = 0;
        if (atomic_compare_exchange_weak(&casLock, &expected, 1))
        {
            return;
        }
    }
}

/*****************************************************************************
 * Name: casRelease
 *
 * Description:
 *         Sets casLock back to 0 (unlocked).
 *
 * Returns:
 *         None.
 *****************************************************************************/
void casRelease(void)
{
    atomic_store(&casLock, 0);
}

/*****************************************************************************
 * Name: threadBody
 *
 * Description:
 *         Repeatedly increments both testAndSetCounter (protected by the
 *         test-and-set spinlock) and casCounter (protected by the CAS
 *         spinlock), so both hardware-based lock implementations are
 *         exercised concurrently in the same run.
 *
 * Inputs:
 *         arg : unused.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *threadBody(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < NUM_ITERATIONS; i++)
    {
        testAndSetAcquire();
        testAndSetCounter++;
        testAndSetRelease();

        casAcquire();
        casCounter++;
        casRelease();
    }
    return NULL;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Spawns NUM_THREADS threads that each increment two counters,
 *         one guarded by a test-and-set spinlock and one guarded by a CAS
 *         spinlock, then verifies both final counts match the expected
 *         total. Contrast with the software-only algorithms in 01-03 of
 *         this folder, which achieve the same mutual exclusion without any
 *         hardware atomic instruction.
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
        pthread_create(&threads[i], NULL, threadBody, NULL);
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("testAndSetCounter = %d (expected %d)\n", testAndSetCounter, expected);
    printf("casCounter        = %d (expected %d)\n", casCounter, expected);

    return 0;
}
