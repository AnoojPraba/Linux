#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define ALARM_SECONDS 2

void handleAlarm(int signum)
{
    (void)signum;
    printf("SIGALRM received - alarm fired\n");
}

int main()
{
    signal(SIGALRM, handleAlarm);

    printf("scheduling alarm in %d seconds\n", ALARM_SECONDS);
    alarm(ALARM_SECONDS);

    pause();

    // raise() sends a signal to the current process itself, useful for
    // testing a handler without waiting on a real timer or external kill.
    printf("now raising SIGALRM manually\n");
    raise(SIGALRM);

    return 0;
}
