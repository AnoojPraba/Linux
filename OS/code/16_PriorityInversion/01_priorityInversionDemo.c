#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#define LOW_PRIORITY_HOLD_USEC 300000
#define MEDIUM_PRIORITY_SPIN_USEC 800000
#define STARTUP_STAGGER_USEC 50000

// Illustrative only - see NOTES.md. Real priority inversion is a timing-
// sensitive scheduler phenomenon; reliably reproducing it requires a
// real-time scheduling policy (SCHED_FIFO/SCHED_RR) and root/CAP_SYS_NICE
// privileges pthread_setschedparam() needs on most systems, so this demo
// mainly exists to be read - the prints tell the priority-inversion story
// via comments even if the exact scheduling doesn't reproduce on every
// machine/permission level. This is the classic scenario behind the 1997
// Mars Pathfinder watchdog-reset bug.
pthread_mutex_t sharedResourceLock = PTHREAD_MUTEX_INITIALIZER;

/*****************************************************************************
 * Name: setThreadPriority
 *
 * Description:
 *         Best-effort attempt to set the calling thread to SCHED_FIFO with
 *         the given priority. Silently leaves the thread at default
 *         scheduling if the call fails (e.g. no CAP_SYS_NICE) - the demo
 *         still runs, just without the real-time priority differences.
 *
 * Inputs:
 *         priority : desired SCHED_FIFO priority.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void setThreadPriority(int priority)
{
    struct sched_param param;

    param.sched_priority = priority;
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0)
    {
        printf("(note: could not set SCHED_FIFO priority %d - "
               "needs CAP_SYS_NICE/root; continuing at default priority)\n", priority);
    }
}

/*****************************************************************************
 * Name: lowPriorityThread
 *
 * Description:
 *         Runs at low priority. Acquires sharedResourceLock and holds it
 *         for a while, simulating a low-priority task that briefly owns a
 *         resource a high-priority task will also need.
 *
 * Inputs:
 *         arg : unused.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *lowPriorityThread(void *arg)
{
    (void)arg;
    setThreadPriority(sched_get_priority_min(SCHED_FIFO));

    printf("[low]    acquiring sharedResourceLock\n");
    pthread_mutex_lock(&sharedResourceLock);
    printf("[low]    holding sharedResourceLock (simulating work)\n");
    usleep(LOW_PRIORITY_HOLD_USEC);
    printf("[low]    releasing sharedResourceLock\n");
    pthread_mutex_unlock(&sharedResourceLock);

    return NULL;
}

/*****************************************************************************
 * Name: mediumPriorityThread
 *
 * Description:
 *         Runs at medium priority and never touches sharedResourceLock.
 *         Simulates CPU-bound work that, on a real-time scheduler without
 *         priority inheritance, would preempt the low-priority thread and
 *         indirectly starve the high-priority thread waiting on it.
 *
 * Inputs:
 *         arg : unused.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *mediumPriorityThread(void *arg)
{
    (void)arg;
    usleep(STARTUP_STAGGER_USEC);
    setThreadPriority((sched_get_priority_min(SCHED_FIFO) +
                        sched_get_priority_max(SCHED_FIFO)) / 2);

    printf("[medium] running CPU-bound work that does not need "
           "sharedResourceLock at all\n");
    usleep(MEDIUM_PRIORITY_SPIN_USEC);
    printf("[medium] done\n");

    return NULL;
}

/*****************************************************************************
 * Name: highPriorityThread
 *
 * Description:
 *         Runs at high priority. Waits to acquire sharedResourceLock,
 *         which the low-priority thread holds. Without priority
 *         inheritance, a medium-priority thread that preempts the
 *         low-priority holder can delay this thread far longer than the
 *         low-priority thread's own critical section would - inverting the
 *         intended priority order.
 *
 * Inputs:
 *         arg : unused.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *highPriorityThread(void *arg)
{
    (void)arg;
    usleep(STARTUP_STAGGER_USEC / 2);
    setThreadPriority(sched_get_priority_max(SCHED_FIFO));

    printf("[high]   waiting for sharedResourceLock (held by [low])\n");
    pthread_mutex_lock(&sharedResourceLock);
    printf("[high]   acquired sharedResourceLock - inversion window over\n");
    pthread_mutex_unlock(&sharedResourceLock);

    return NULL;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Spawns low/medium/high "priority" threads in that order to walk
 *         through the priority-inversion scenario: low grabs the lock,
 *         high blocks waiting on it, medium (which needs no lock) can then
 *         preempt low and delay it - and transitively delay high - longer
 *         than expected.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    pthread_t low;
    pthread_t medium;
    pthread_t high;

    printf("priority inversion demo (illustrative - see NOTES.md)\n");

    pthread_create(&low, NULL, lowPriorityThread, NULL);
    pthread_create(&high, NULL, highPriorityThread, NULL);
    pthread_create(&medium, NULL, mediumPriorityThread, NULL);

    pthread_join(low, NULL);
    pthread_join(medium, NULL);
    pthread_join(high, NULL);

    printf("done\n");
    return 0;
}
