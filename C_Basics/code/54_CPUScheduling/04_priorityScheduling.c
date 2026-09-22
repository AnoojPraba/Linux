#include <stdio.h>

#define NUM_PROCESSES 4

typedef struct
{
    int pid;
    int burstTime;
    int priority;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
    int done;
} Process;

// Priority scheduling: always run the highest-priority ready process next
// (lower number = higher priority here) - structurally identical to SJF's
// "always pick the best candidate" loop, just comparing priority instead
// of burst time. A real OS pairs this with aging (temporarily boosting a
// long-waiting process's priority) to avoid starving low-priority work
// indefinitely, which this simplified version doesn't implement.
void priorityScheduling(Process processes[], int n)
{
    int currentTime = 0;
    int completed = 0;

    while (completed < n)
    {
        int bestIndex = -1;
        int i;

        for (i = 0; i < n; i++)
        {
            if (!processes[i].done)
            {
                if ((bestIndex == -1) ||
                    (processes[i].priority < processes[bestIndex].priority))
                {
                    bestIndex = i;
                }
            }
        }

        currentTime += processes[bestIndex].burstTime;
        processes[bestIndex].completionTime = currentTime;
        processes[bestIndex].turnaroundTime = processes[bestIndex].completionTime;
        processes[bestIndex].waitingTime =
            processes[bestIndex].turnaroundTime - processes[bestIndex].burstTime;
        processes[bestIndex].done = 1;
        completed++;
    }
}

void printResults(Process processes[], int n)
{
    int i;
    double totalWaiting = 0;

    printf("PID\tPriority\tBurst\tCompletion\tWaiting\tTurnaround\n");
    for (i = 0; i < n; i++)
    {
        printf("%d\t%d\t\t%d\t%d\t\t%d\t%d\n", processes[i].pid, processes[i].priority,
               processes[i].burstTime, processes[i].completionTime,
               processes[i].waitingTime, processes[i].turnaroundTime);
        totalWaiting += processes[i].waitingTime;
    }
    printf("average waiting time = %.2f\n", totalWaiting / n);
}

int main()
{
    Process processes[NUM_PROCESSES] = {
        {1, 5, 2, 0, 0, 0, 0}, {2, 3, 1, 0, 0, 0, 0},
        {3, 8, 4, 0, 0, 0, 0}, {4, 6, 3, 0, 0, 0, 0}
    };

    priorityScheduling(processes, NUM_PROCESSES);
    printResults(processes, NUM_PROCESSES);

    return 0;
}
