#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t interrupted = 0;

// A signal handler can be invoked at almost any point in program execution,
// so it should only touch a sig_atomic_t/volatile flag and return quickly -
// not call non-reentrant functions like printf.
void handleSigint(int signum)
{
    (void)signum;
    interrupted = 1;
}

int main()
{
    struct sigaction sa;

    sa.sa_handler = handleSigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    printf("running (send SIGINT with kill -INT %d, or press Ctrl+C within 5s)\n",
           getpid());

    int secondsWaited = 0;

    while ((!interrupted) && (secondsWaited < 5))
    {
        sleep(1);
        secondsWaited++;
    }

    if (interrupted)
    {
        printf("caught SIGINT, exiting cleanly\n");
    }
    else
    {
        printf("no signal received, exiting after timeout\n");
    }

    return 0;
}
