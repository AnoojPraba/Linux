#include <stdio.h>

#define NUM_PROCESSES 4

typedef struct
{
    int pid;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
    int done;
} Process;

// Shortest Job First (non-preemptive): among processes that have already
// arrived, always run the one with the smallest remaining burst time next -
// minimizes average waiting time versus FCFS, at the cost of needing to
// know each burst time in advance (rarely true in a real OS) and
// potentially starving long processes if short ones keep arriving.
void sjf(Process processes[], int n)
{
    int currentTime = 0;
    int completed = 0;

    while (completed < n)
    {
        int shortestIndex = -1;
        int i;

        for (i = 0; i < n; i++)
        {
            if ((!processes[i].done) && (processes[i].arrivalTime <= currentTime))
            {
                if ((shortestIndex == -1) ||
                    (processes[i].burstTime < processes[shortestIndex].burstTime))
                {
                    shortestIndex = i;
                }
            }
        }

        if (shortestIndex == -1)
        {
            // Nothing has arrived yet - jump forward to the next arrival.
            currentTime++;
            continue;
        }

        currentTime += processes[shortestIndex].burstTime;
        processes[shortestIndex].completionTime = currentTime;
        processes[shortestIndex].turnaroundTime =
            processes[shortestIndex].completionTime - processes[shortestIndex].arrivalTime;
        processes[shortestIndex].waitingTime =
            processes[shortestIndex].turnaroundTime - processes[shortestIndex].burstTime;
        processes[shortestIndex].done = 1;
        completed++;
    }
}

void printResults(Process processes[], int n)
{
    int i;
    double totalWaiting = 0;

    printf("PID\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
    for (i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n", processes[i].pid,
               processes[i].arrivalTime, processes[i].burstTime,
               processes[i].completionTime, processes[i].waitingTime,
               processes[i].turnaroundTime);
        totalWaiting += processes[i].waitingTime;
    }
    printf("average waiting time = %.2f\n", totalWaiting / n);
}

int main()
{
    Process processes[NUM_PROCESSES] = {
        {1, 0, 5, 0, 0, 0, 0}, {2, 1, 3, 0, 0, 0, 0},
        {3, 2, 8, 0, 0, 0, 0}, {4, 3, 6, 0, 0, 0, 0}
    };

    sjf(processes, NUM_PROCESSES);
    printResults(processes, NUM_PROCESSES);

    return 0;
}
