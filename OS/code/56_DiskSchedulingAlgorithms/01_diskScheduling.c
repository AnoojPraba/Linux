#include <stdio.h>
#include <stdlib.h>

#define DISK_SIZE     200
#define HEAD_START    53
#define NUM_REQUESTS  8

/*****************************************************************************
 * Name: printOrder
 *
 * Description:
 *         Prints the algorithm name and the service order it produced.
 *
 * Inputs:
 *         name  : algorithm name to label the output with.
 *         order : array of cylinder numbers in the order they were serviced.
 *         count : number of entries in order.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void printOrder(const char *name, const int *order, int count)
{
    int i;

    printf("%s order: ", name);
    for (i = 0; i < count; i++)
    {
        printf("%d ", order[i]);
    }
    printf("\n");
}

/*****************************************************************************
 * Name: fcfs
 *
 * Description:
 *         First-Come-First-Served - services requests strictly in the order
 *         they appear in the request array, regardless of head position.
 *
 * Inputs:
 *         requests : array of requested cylinder numbers.
 *         count    : number of requests.
 *         head     : starting head position.
 *
 * Returns:
 *         Total head movement (sum of absolute seek distances).
 *****************************************************************************/
int fcfs(const int *requests, int count, int head)
{
    int order[NUM_REQUESTS];
    int total = 0;
    int i;

    for (i = 0; i < count; i++)
    {
        order[i] = requests[i];
        total += abs(order[i] - head);
        head = order[i];
    }

    printOrder("FCFS", order, count);
    return total;
}

/*****************************************************************************
 * Name: sstf
 *
 * Description:
 *         Shortest Seek Time First - repeatedly picks the not-yet-serviced
 *         request closest to the current head position.
 *
 * Inputs:
 *         requests : array of requested cylinder numbers.
 *         count    : number of requests.
 *         head     : starting head position.
 *
 * Returns:
 *         Total head movement (sum of absolute seek distances).
 *****************************************************************************/
int sstf(const int *requests, int count, int head)
{
    int remaining[NUM_REQUESTS];
    int done[NUM_REQUESTS] = { 0 };
    int order[NUM_REQUESTS];
    int total = 0;
    int i;
    int j;

    for (i = 0; i < count; i++)
    {
        remaining[i] = requests[i];
    }

    for (i = 0; i < count; i++)
    {
        int bestIdx = -1;
        int bestDist = 0;

        for (j = 0; j < count; j++)
        {
            if (!done[j])
            {
                int dist = abs(remaining[j] - head);

                if ((bestIdx == -1) || (dist < bestDist))
                {
                    bestIdx = j;
                    bestDist = dist;
                }
            }
        }

        done[bestIdx] = 1;
        order[i] = remaining[bestIdx];
        total += bestDist;
        head = remaining[bestIdx];
    }

    printOrder("SSTF", order, count);
    return total;
}

/*****************************************************************************
 * Name: compareInts
 *
 * Description:
 *         Comparator for qsort() over an array of ints, ascending order.
 *
 * Inputs:
 *         a : pointer to first int.
 *         b : pointer to second int.
 *
 * Returns:
 *         Negative, zero, or positive per standard qsort() comparator rules.
 *****************************************************************************/
int compareInts(const void *a, const void *b)
{
    return (*(const int *)a - *(const int *)b);
}

/*****************************************************************************
 * Name: scan
 *
 * Description:
 *         SCAN (elevator) algorithm - sweeps toward the high end of the disk
 *         servicing every pending request passed along the way, then
 *         reverses and sweeps back servicing the rest.
 *
 * Inputs:
 *         requests : array of requested cylinder numbers.
 *         count    : number of requests.
 *         head     : starting head position.
 *         diskSize : number of cylinders on the disk (0..diskSize - 1).
 *
 * Returns:
 *         Total head movement (sum of absolute seek distances).
 *****************************************************************************/
int scan(const int *requests, int count, int head, int diskSize)
{
    int sorted[NUM_REQUESTS];
    int order[NUM_REQUESTS];
    int total = 0;
    int outIdx = 0;
    int i;
    int lastPos = head;

    for (i = 0; i < count; i++)
    {
        sorted[i] = requests[i];
    }
    qsort(sorted, count, sizeof(int), compareInts);

    // service everything at or above head, moving upward, then hit the end
    for (i = 0; i < count; i++)
    {
        if (sorted[i] >= head)
        {
            order[outIdx++] = sorted[i];
        }
    }
    total += abs((diskSize - 1) - head);
    lastPos = diskSize - 1;

    // reverse and service everything below head, from highest down to lowest
    for (i = count - 1; i >= 0; i--)
    {
        if (sorted[i] < head)
        {
            order[outIdx++] = sorted[i];
            total += abs(lastPos - sorted[i]);
            lastPos = sorted[i];
        }
    }

    printOrder("SCAN", order, outIdx);
    return total;
}

/*****************************************************************************
 * Name: cscan
 *
 * Description:
 *         Circular SCAN - sweeps toward the high end of the disk servicing
 *         pending requests along the way, then jumps back to cylinder 0
 *         without servicing anything, and resumes sweeping upward through
 *         the remaining requests.
 *
 * Inputs:
 *         requests : array of requested cylinder numbers.
 *         count    : number of requests.
 *         head     : starting head position.
 *         diskSize : number of cylinders on the disk (0..diskSize - 1).
 *
 * Returns:
 *         Total head movement (sum of absolute seek distances).
 *****************************************************************************/
int cscan(const int *requests, int count, int head, int diskSize)
{
    int sorted[NUM_REQUESTS];
    int order[NUM_REQUESTS];
    int total = 0;
    int outIdx = 0;
    int i;
    int lastPos = head;

    for (i = 0; i < count; i++)
    {
        sorted[i] = requests[i];
    }
    qsort(sorted, count, sizeof(int), compareInts);

    // service everything at or above head, moving upward, then hit the end
    for (i = 0; i < count; i++)
    {
        if (sorted[i] >= head)
        {
            order[outIdx++] = sorted[i];
            total += abs(sorted[i] - lastPos);
            lastPos = sorted[i];
        }
    }
    total += abs((diskSize - 1) - lastPos);

    // circular jump back to the start (unserviced), then sweep up again
    total += (diskSize - 1);
    lastPos = 0;

    for (i = 0; i < count; i++)
    {
        if (sorted[i] < head)
        {
            order[outIdx++] = sorted[i];
            total += abs(sorted[i] - lastPos);
            lastPos = sorted[i];
        }
    }

    printOrder("C-SCAN", order, outIdx);
    return total;
}

int main(void)
{
    int requests[NUM_REQUESTS] = { 98, 183, 37, 122, 14, 124, 65, 67 };

    printf("Head start: %d, disk size: %d\n", HEAD_START, DISK_SIZE);

    printf("FCFS total seek distance:   %d\n",
           fcfs(requests, NUM_REQUESTS, HEAD_START));
    printf("SSTF total seek distance:   %d\n",
           sstf(requests, NUM_REQUESTS, HEAD_START));
    printf("SCAN total seek distance:   %d\n",
           scan(requests, NUM_REQUESTS, HEAD_START, DISK_SIZE));
    printf("C-SCAN total seek distance: %d\n",
           cscan(requests, NUM_REQUESTS, HEAD_START, DISK_SIZE));

    return 0;
}
