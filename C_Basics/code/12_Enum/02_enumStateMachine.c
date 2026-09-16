#include <stdio.h>

enum State
{
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_STOPPED
};

const char *stateName(enum State s)
{
    switch (s)
    {
        case STATE_IDLE:
            return "IDLE";

        case STATE_RUNNING:
            return "RUNNING";

        case STATE_PAUSED:
            return "PAUSED";

        case STATE_STOPPED:
            return "STOPPED";

        default:
            return "UNKNOWN";
    }
}

enum State nextState(enum State current)
{
    switch (current)
    {
        case STATE_IDLE:
            return STATE_RUNNING;

        case STATE_RUNNING:
            return STATE_PAUSED;

        case STATE_PAUSED:
            return STATE_RUNNING;

        default:
            return STATE_STOPPED;
    }
}

int main()
{
    enum State s = STATE_IDLE;
    int i;

    for (i = 0; i < 4; i++)
    {
        printf("state: %s\n", stateName(s));
        s = nextState(s);
    }

    return 0;
}
