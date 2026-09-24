#include <stdio.h>

#define NUM_PROCESSES 4
#define TIME_QUANTUM 3
#define MAX_QUEUE_SIZE 128

typedef struct
{
    int pid;
    int burstTime;
    int remainingTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
} Process;

// Round Robin: each process gets a fixed time slice (the quantum), then is
// preempted and sent to the back of the queue if it isn't finished yet -
// unlike FCFS/SJF, which always run a process to completion once started.
// This bounds the worst-case wait any single process experiences, at the
// cost of more context switches.
void roundRobin(Process processes[], int n, int quantum)
{
    int queue[MAX_QUEUE_SIZE];
    int front = 0;
    int back = 0;
    int currentTime = 0;
    int i;

    for (i = 0; i < n; i++)
    {
        queue[back++] = i;
    }

    while (front < back)
    {
        int index = queue[front++];
        int runTime = (processes[index].remainingTime < quantum)
                          ? processes[index].remainingTime
                          : quantum;

        currentTime += runTime;
        processes[index].remainingTime -= runTime;

        if (processes[index].remainingTime > 0)
        {
            queue[back++] = index;
        }
        else
        {
            processes[index].completionTime = currentTime;
            processes[index].turnaroundTime = processes[index].completionTime;
            processes[index].waitingTime =
                processes[index].turnaroundTime - processes[index].burstTime;
        }
    }
}

void printResults(Process processes[], int n)
{
    int i;
    double totalWaiting = 0;

    printf("PID\tBurst\tCompletion\tWaiting\tTurnaround\n");
    for (i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t\t%d\t%d\n", processes[i].pid, processes[i].burstTime,
               processes[i].completionTime, processes[i].waitingTime,
               processes[i].turnaroundTime);
        totalWaiting += processes[i].waitingTime;
    }
    printf("average waiting time = %.2f\n", totalWaiting / n);
}

int main()
{
    Process processes[NUM_PROCESSES] = {
        {1, 5, 5, 0, 0, 0}, {2, 3, 3, 0, 0, 0}, {3, 8, 8, 0, 0, 0}, {4, 6, 6, 0, 0, 0}
    };

    roundRobin(processes, NUM_PROCESSES, TIME_QUANTUM);
    printResults(processes, NUM_PROCESSES);

    return 0;
}
