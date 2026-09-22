#include <stdio.h>
#include <sched.h>
#include <time.h>

#define NUM_YIELDS 100000

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Times a large number of sched_yield() calls to give an empirical
 *         feel for context switch cost. Each sched_yield() typically forces
 *         the scheduler to run (and this process to be rescheduled),
 *         approximating one voluntary context switch per call.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    struct timespec start;
    struct timespec end;
    long i;
    double totalSeconds;
    double perYieldNanos;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < NUM_YIELDS; i++)
    {
        sched_yield();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    totalSeconds = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    perYieldNanos = (totalSeconds * 1e9) / NUM_YIELDS;

    printf("%d sched_yield() calls took %.6f seconds\n", NUM_YIELDS, totalSeconds);
    printf("approx %.1f ns per yield (rough proxy for context switch cost)\n",
           perYieldNanos);

    return 0;
}
