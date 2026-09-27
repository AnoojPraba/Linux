#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// A POSIX signal handler is the closest thing to a real ISR demonstrable in
// userspace C. It is NOT a real embedded ISR: a real embedded ISR runs with
// additional hardware-specific constraints this demo cannot show (see the
// comment block in main() below). `volatile sig_atomic_t` for the
// handler/main-loop shared flag is the same pattern already covered in
// `11_VolatileVsAtomicEmbedded/01_volatileNotAtomic.c` - not repeated here,
// just reused correctly.
static volatile sig_atomic_t dataReadyFlag = 0;
static volatile sig_atomic_t lastSignalValue = 0;

#define MAIN_LOOP_ITERATIONS 3
#define MAIN_LOOP_SLEEP_SEC 1

// RULE: keep the handler minimal. It only sets a flag and copies the
// minimal piece of data (the signal number) needed by the main loop - it
// does NOT do the real work itself. The actual (potentially slow) work is
// deferred to the main loop, which checks the flag and acts on it.
void handleSignal(int signum)
{
    lastSignalValue = signum;
    dataReadyFlag = 1;
}

// This is the "real work" that a genuine ISR must NOT do inline - it is only
// safe to run here, in the main loop, after the flag has been observed.
void processDeferredWork(int signum)
{
    printf("main loop: processing deferred work for signal %d\n", signum);
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates the two rules a signal handler can illustrate in
 *         userspace: (1) use volatile sig_atomic_t for handler/main-loop
 *         shared state, and (2) keep the handler body minimal, deferring
 *         real work to the main loop. Rules a real embedded ISR must also
 *         follow, which this userspace demo cannot fully illustrate:
 *           - never call a blocking function (e.g. one that waits on I/O
 *             or a lock) - the whole system may be waiting on this ISR.
 *           - never call a non-reentrant function - most of the standard C
 *             library (including malloc()/printf()) is not guaranteed
 *             async-signal-safe/reentrant, so calling it from a real ISR
 *             can corrupt state it shares with interrupted "normal" code.
 *           - keep execution time as short as possible - the ISR blocks
 *             other interrupts (or at least that interrupt line) for its
 *             entire duration.
 *           - don't use floating point on architectures where the FPU
 *             context isn't automatically saved/restored on interrupt
 *             entry/exit, unless it is saved/restored manually.
 *           - clear/acknowledge the interrupt source register before
 *             returning, or the same interrupt immediately re-fires.
 *         This demo's own handleSignal() breaks the "no printf" rule only
 *         in spirit for isr-benign-here reasons: it doesn't call printf(),
 *         but main() below does, off the main loop, only after the flag
 *         is observed.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    int i = 0;

    signal(SIGUSR1, handleSignal);

    printf("pid = %d, send SIGUSR1 to trigger deferred work "
           "(e.g. `kill -USR1 %d`)\n", getpid(), getpid());

    for (i = 0; i < MAIN_LOOP_ITERATIONS; i++)
    {
        if (dataReadyFlag)
        {
            dataReadyFlag = 0;
            processDeferredWork(lastSignalValue);
        }
        else
        {
            printf("main loop: no signal yet (%d/%d)\n", i + 1, MAIN_LOOP_ITERATIONS);
        }

        sleep(MAIN_LOOP_SLEEP_SEC);
    }

    return 0;
}
