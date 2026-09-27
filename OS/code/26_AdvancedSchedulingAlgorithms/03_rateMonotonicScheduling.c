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

// Rate Monotonic Scheduling: static priority assigned inversely to period -
// the task with the SHORTEST period gets the HIGHEST priority, and this
// priority never changes at runtime. At every tick, the ready task with the
// shortest period is chosen to run. Liu & Layland proved RMS is optimal
// among static-priority algorithms, and gave a sufficient (not necessary)
// utilization bound: a task set of n tasks is guaranteed schedulable if
// sum(wcet_i / period_i) <= n * (2^(1/n) - 1), which converges to about 69%
// as n grows. Failing the bound does not prove the task set is
// unschedulable under RMS - it just means the bound can't guarantee it.
void rateMonotonicSimulate(Task tasks[], int n)
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
                if ((chosen == -1) || (tasks[i].period < tasks[chosen].period))
                {
                    chosen = i;
                }
            }
        }

        if (chosen != -1)
        {
            tasks[chosen].remainingTime--;
            printf("t=%d task %d runs (remaining=%d)\n", t, chosen,
                   tasks[chosen].remainingTime);
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
    Task tasks[NUM_TASKS] = {{20, 3, 0, 0, 0, 0}, {30, 6, 0, 0, 0, 0}, {60, 10, 0, 0, 0, 0}};
    int i;
    int allMet = 1;
    int utilizationScaled = 0;
    int boundScaled;

    for (i = 0; i < NUM_TASKS; i++)
    {
        utilizationScaled += (tasks[i].wcet * UTILIZATION_SCALE) / tasks[i].period;
    }

    // n * (2^(1/n) - 1) for n = 1..5, precomputed and scaled by UTILIZATION_SCALE,
    // since C89 has no easy fixed way to compute a general n-th root at runtime
    // without <math.h> pow(); this table covers the small task-set sizes used here.
    {
        static const int BOUND_SCALED_FOR_N[] = {1000, 828, 780, 757, 743};

        boundScaled = BOUND_SCALED_FOR_N[NUM_TASKS - 1];
    }

    printf("Task set: period/wcet = ");
    for (i = 0; i < NUM_TASKS; i++)
    {
        printf("(%d/%d) ", tasks[i].period, tasks[i].wcet);
    }
    printf("\n");

    printf("Utilization = %d.%03d, RMS bound for n=%d = %d.%03d\n",
           utilizationScaled / UTILIZATION_SCALE, utilizationScaled % UTILIZATION_SCALE,
           NUM_TASKS, boundScaled / UTILIZATION_SCALE, boundScaled % UTILIZATION_SCALE);

    if (utilizationScaled <= boundScaled)
    {
        printf("RMS utilization bound: PASSES (schedulability guaranteed)\n");
    }
    else
    {
        printf("RMS utilization bound: FAILS (not guaranteed by the bound)\n");
    }

    rateMonotonicSimulate(tasks, NUM_TASKS);

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
