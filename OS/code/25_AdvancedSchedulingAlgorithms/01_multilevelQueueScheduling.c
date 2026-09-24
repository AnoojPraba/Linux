#include <stdio.h>

#define NUM_SYSTEM_PROCESSES 2
#define NUM_INTERACTIVE_PROCESSES 2
#define NUM_BATCH_PROCESSES 2
#define BATCH_TIME_QUANTUM 4

typedef struct
{
    int pid;
    int burstTime;
    int remainingTime;
    int completionTime;
} Process;

// Multilevel Queue Scheduling: processes are permanently classified into
// separate queues by category (here: system, interactive, batch), and each
// queue can run its own algorithm. Between queues there is a fixed
// priority - every system process runs to completion before any
// interactive process is even considered, and likewise interactive before
// batch. This is simple and gives strong guarantees to high-priority
// categories, but a process can never move between queues once assigned,
// so a misclassified or newly-behaving process is stuck with its initial
// category forever (the gap 02_multilevelFeedbackQueue.c addresses).
void runSystemQueue(Process processes[], int n, int *currentTime)
{
    int i;

    // System queue: FCFS, highest fixed priority of all three queues.
    for (i = 0; i < n; i++)
    {
        *currentTime += processes[i].burstTime;
        processes[i].completionTime = *currentTime;
        printf("[system]      pid=%d finished at t=%d\n", processes[i].pid,
               processes[i].completionTime);
    }
}

void runInteractiveQueue(Process processes[], int n, int *currentTime)
{
    int i;

    // Interactive queue: FCFS as well, but only ever runs once every
    // system process has completed.
    for (i = 0; i < n; i++)
    {
        *currentTime += processes[i].burstTime;
        processes[i].completionTime = *currentTime;
        printf("[interactive] pid=%d finished at t=%d\n", processes[i].pid,
               processes[i].completionTime);
    }
}

void runBatchQueue(Process processes[], int n, int quantum, int *currentTime)
{
    int queue[NUM_BATCH_PROCESSES * 8];
    int front = 0;
    int back = 0;
    int i;

    // Batch queue: Round Robin, lowest fixed priority - only runs once
    // both the system and interactive queues are fully drained.
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

        *currentTime += runTime;
        processes[index].remainingTime -= runTime;

        if (processes[index].remainingTime > 0)
        {
            queue[back++] = index;
        }
        else
        {
            processes[index].completionTime = *currentTime;
            printf("[batch]       pid=%d finished at t=%d\n", processes[index].pid,
                   processes[index].completionTime);
        }
    }
}

int main()
{
    Process systemQueue[NUM_SYSTEM_PROCESSES] = {{1, 2, 2, 0}, {2, 3, 3, 0}};
    Process interactiveQueue[NUM_INTERACTIVE_PROCESSES] = {{3, 4, 4, 0}, {4, 2, 2, 0}};
    Process batchQueue[NUM_BATCH_PROCESSES] = {{5, 10, 10, 0}, {6, 6, 6, 0}};
    int currentTime = 0;

    runSystemQueue(systemQueue, NUM_SYSTEM_PROCESSES, &currentTime);
    runInteractiveQueue(interactiveQueue, NUM_INTERACTIVE_PROCESSES, &currentTime);
    runBatchQueue(batchQueue, NUM_BATCH_PROCESSES, BATCH_TIME_QUANTUM, &currentTime);

    return 0;
}
