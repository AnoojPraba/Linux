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
} Process;

// First-Come, First-Served: processes run strictly in arrival order, each
// running to completion before the next starts - the simplest scheduling
// policy, but a short process arriving just after a long one still waits
// for the whole long burst (the "convoy effect").
void fcfs(Process processes[], int n)
{
    int currentTime = 0;
    int i;

    for (i = 0; i < n; i++)
    {
        if (currentTime < processes[i].arrivalTime)
        {
            currentTime = processes[i].arrivalTime;
        }

        currentTime += processes[i].burstTime;
        processes[i].completionTime = currentTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
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
        {1, 0, 5, 0, 0, 0}, {2, 1, 3, 0, 0, 0}, {3, 2, 8, 0, 0, 0}, {4, 3, 6, 0, 0, 0}
    };

    fcfs(processes, NUM_PROCESSES);
    printResults(processes, NUM_PROCESSES);

    return 0;
}
