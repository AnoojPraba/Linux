#include <stdio.h>

#define NUM_TASKS 3
#define HYPERPERIOD 60
#define UTILIZATION_SCALE 1000

typedef struct
{
    int period;
    int wcet;
    int remainingTime;
    int nextRelease;
    int absoluteDeadline;
    int missedDeadline;
} Task;

// Earliest Deadline First: dynamic priority - at every tick, whichever ready
// task has the nearest ABSOLUTE deadline runs, so priority order can change
// every time a task is released or finishes. EDF is optimal among ALL
// scheduling algorithms (not just static-priority ones like RMS): a periodic
// task set is schedulable under EDF if and only if total utilization
// sum(wcet_i / period_i) <= 1 (100%) - a simpler and tighter bound than RMS's
// n * (2^(1/n) - 1), which converges to only about 69%. The task set below is
// picked so its utilization sits between the RMS bound and 1.0, demonstrating
// that EDF's guarantee covers cases RMS's bound cannot - not that RMS itself
// necessarily fails on this particular set.
void earliestDeadlineFirstSimulate(Task tasks[], int n)
{
    int t;
    int i;

    for (i = 0; i < n; i++)
    {
        tasks[i].remainingTime = 0;
        tasks[i].nextRelease = 0;
        tasks[i].absoluteDeadline = tasks[i].period;
        tasks[i].missedDeadline = 0;
    }

    for (t = 0; t < HYPERPERIOD; t++)
    {
        int chosen = -1;

        for (i = 0; i < n; i++)
        {
            if (t == tasks[i].nextRelease)
            {
                if (tasks[i].remainingTime > 0)
                {
                    printf("t=%d task %d missed deadline at release\n", t, i);
                    tasks[i].missedDeadline = 1;
                }

                tasks[i].remainingTime = tasks[i].wcet;
                tasks[i].nextRelease += tasks[i].period;
                tasks[i].absoluteDeadline = t + tasks[i].period;
            }
        }

        for (i = 0; i < n; i++)
        {
            if (tasks[i].remainingTime > 0)
            {
                if ((chosen == -1) || (tasks[i].absoluteDeadline < tasks[chosen].absoluteDeadline))
                {
                    chosen = i;
                }
            }
        }

        if (chosen != -1)
        {
            tasks[chosen].remainingTime--;
            printf("t=%d task %d runs (deadline=%d, remaining=%d)\n", t, chosen,
                   tasks[chosen].absoluteDeadline, tasks[chosen].remainingTime);
        }
        else
        {
            printf("t=%d idle\n", t);
        }

        for (i = 0; i < n; i++)
        {
            if ((t + 1 == tasks[i].absoluteDeadline) && (tasks[i].remainingTime > 0))
            {
                printf("t=%d task %d missed deadline at t=%d\n", t + 1, i,
                       tasks[i].absoluteDeadline);
                tasks[i].missedDeadline = 1;
            }
        }
    }
}

int main()
{
    Task tasks[NUM_TASKS] = {{20, 6, 0, 0, 0, 0}, {30, 9, 0, 0, 0, 0}, {60, 18, 0, 0, 0, 0}};
    int i;
    int allMet = 1;
    int utilizationScaled = 0;

    // Same RMS bound table as 03_rateMonotonicScheduling.c, for n = 1..5.
    static const int BOUND_SCALED_FOR_N[] = {1000, 828, 780, 757, 743};
    int rmsBoundScaled = BOUND_SCALED_FOR_N[NUM_TASKS - 1];

    for (i = 0; i < NUM_TASKS; i++)
    {
        utilizationScaled += (tasks[i].wcet * UTILIZATION_SCALE) / tasks[i].period;
    }

    printf("Task set: period/wcet = ");
    for (i = 0; i < NUM_TASKS; i++)
    {
        printf("(%d/%d) ", tasks[i].period, tasks[i].wcet);
    }
    printf("\n");

    printf("Utilization = %d.%03d, RMS bound for n=%d = %d.%03d, EDF bound = 1.000\n",
           utilizationScaled / UTILIZATION_SCALE, utilizationScaled % UTILIZATION_SCALE,
           NUM_TASKS, rmsBoundScaled / UTILIZATION_SCALE, rmsBoundScaled % UTILIZATION_SCALE);

    if (utilizationScaled <= rmsBoundScaled)
    {
        printf("RMS utilization bound: PASSES\n");
    }
    else
    {
        printf("RMS utilization bound: FAILS (not guaranteed by the bound)\n");
    }

    if (utilizationScaled <= UTILIZATION_SCALE)
    {
        printf("EDF utilization bound (<=100%%): PASSES (schedulability guaranteed)\n");
    }
    else
    {
        printf("EDF utilization bound (<=100%%): FAILS (overloaded, deadlines will be missed)\n");
    }

    earliestDeadlineFirstSimulate(tasks, NUM_TASKS);

    for (i = 0; i < NUM_TASKS; i++)
    {
        if (tasks[i].missedDeadline)
        {
            allMet = 0;
        }
    }

    printf("Simulation result: %s\n", allMet ? "all deadlines met" : "a deadline was missed");

    return 0;
}
