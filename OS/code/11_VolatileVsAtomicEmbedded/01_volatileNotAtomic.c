#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// volatile only tells the compiler "do not cache this in a register / do not
// reorder or eliminate accesses" - it says nothing about atomicity or memory
// ordering across threads/cores. It is the right tool for a flag touched by
// a signal handler on the same thread, not for cross-thread synchronization.
static volatile sig_atomic_t stopRequested = 0;

#define POLL_LIMIT 3

void handleSigint(int signum)
{
    (void)signum;
    stopRequested = 1;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates the correct, narrow use case for volatile: a flag set
 *         by a signal handler and polled by the main loop. sig_atomic_t is
 *         used because it is guaranteed safe to read/write from a signal
 *         handler without tearing.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    int polls = 0;

    signal(SIGINT, handleSigint);

    printf("polling stopRequested flag (send SIGINT to stop early)...\n");
    while ((!stopRequested) && (polls < POLL_LIMIT))
    {
        printf("poll %d: stopRequested = %d\n", polls, stopRequested);
        sleep(1);
        polls++;
    }

    printf("done, stopRequested = %d\n", stopRequested);
    return 0;
}
