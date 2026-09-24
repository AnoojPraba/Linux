#include <stdio.h>

#define NUM_PROCESSES 3
#define NUM_LEVELS 3
#define MAX_QUEUE_SIZE 32

// Quantum grows at lower-priority levels: level 0 is short and interactive,
// level 2 is long and batch-like.
static const int QUANTUM_FOR_LEVEL[NUM_LEVELS] = {2, 4, 8};

typedef struct
{
    int pid;
    int remainingTime;
    int level;
} Process;

// Multilevel Feedback Queue: like MLQ, but a process is not permanently
// assigned to a queue - it moves between levels based on observed
// behavior. A process that uses its ENTIRE quantum without blocking is
// demoted to a lower-priority, longer-quantum level (it's probably
// CPU-bound); a process that blocks for I/O before its quantum expires
// stays at (or is promoted back toward) the high-priority level (it's
// probably interactive). This is MLFQ's key advantage over plain MLQ: the
// scheduler discovers a process's category from its actual runtime
// behavior instead of needing to know it upfront.
void multilevelFeedbackQueue(Process processes[], int n)
{
    int queues[NUM_LEVELS][MAX_QUEUE_SIZE];
    int queueFront[NUM_LEVELS] = {0};
    int queueBack[NUM_LEVELS] = {0};
    int currentTime = 0;
    int remainingProcesses = n;
    int i;
    int level;

    for (i = 0; i < n; i++)
    {
        queues[0][queueBack[0]++] = i;
    }

    while (remainingProcesses > 0)
    {
        for (level = 0; level < NUM_LEVELS; level++)
        {
            if (queueFront[level] < queueBack[level])
            {
                int index = queues[level][queueFront[level]++];
                int quantum = QUANTUM_FOR_LEVEL[level];
                int runTime = (processes[index].remainingTime < quantum)
                                  ? processes[index].remainingTime
                                  : quantum;

                currentTime += runTime;
                processes[index].remainingTime -= runTime;

                if (processes[index].remainingTime == 0)
                {
                    printf("t=%d pid=%d finished at level %d\n", currentTime,
                           processes[index].pid, level);
                    remainingProcesses--;
                }
                else if (runTime == quantum)
                {
                    // Used the full quantum without finishing - demote to the
                    // next (lower-priority, longer-quantum) level.
                    int nextLevel = (level + 1 < NUM_LEVELS) ? level + 1 : level;

                    printf("t=%d pid=%d used full quantum, demoted level %d -> %d\n",
                           currentTime, processes[index].pid, level, nextLevel);
                    processes[index].level = nextLevel;
                    queues[nextLevel][queueBack[nextLevel]++] = index;
                }
                else
                {
                    // Blocked before the quantum expired - stays at this level.
                    printf("t=%d pid=%d yielded early, stays at level %d\n", currentTime,
                           processes[index].pid, level);
                    queues[level][queueBack[level]++] = index;
                }

                break;
            }
        }
    }
}

int main()
{
    Process processes[NUM_PROCESSES] = {{1, 10, 0}, {2, 3, 0}, {3, 6, 0}};

    multilevelFeedbackQueue(processes, NUM_PROCESSES);

    return 0;
}
