#include <stdio.h>

#define NUM_INTERVALS 4

typedef struct
{
    int start;
    int end;
} Interval;

void sortIntervals(Interval intervals[], int n)
{
    int i;
    int j;
    Interval temp;

    for (i = 0; i < (n - 1); i++)
    {
        for (j = 0; j < (n - 1 - i); j++)
        {
            if (intervals[j].start > intervals[j + 1].start)
            {
                temp = intervals[j];
                intervals[j] = intervals[j + 1];
                intervals[j + 1] = temp;
            }
        }
    }
}

/*****************************************************************************
 * Name: mergeIntervals
 *
 * Description:
 *         Merges all overlapping intervals: sort by start time, then sweep
 *         through, extending the current merged interval's end whenever the
 *         next interval's start is <= that end (they overlap or touch), or
 *         starting a fresh merged interval otherwise.
 *
 * Inputs:
 *         intervals : array of intervals, sorted in place by this function.
 *         n         : number of intervals.
 *         result    : out-param array (same size as intervals) receiving
 *                     the merged intervals.
 *
 * Returns:
 *         Number of merged intervals written to result.
 *****************************************************************************/
int mergeIntervals(Interval intervals[], int n, Interval result[])
{
    int resultCount;
    int i;

    if (n == 0)
    {
        return 0;
    }

    sortIntervals(intervals, n);

    result[0] = intervals[0];
    resultCount = 1;

    for (i = 1; i < n; i++)
    {
        if (intervals[i].start <= result[resultCount - 1].end)
        {
            if (intervals[i].end > result[resultCount - 1].end)
            {
                result[resultCount - 1].end = intervals[i].end;
            }
        }
        else
        {
            result[resultCount] = intervals[i];
            resultCount++;
        }
    }

    return resultCount;
}

int main(void)
{
    Interval intervals[NUM_INTERVALS] = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};
    Interval result[NUM_INTERVALS];
    int count;
    int i;

    count = mergeIntervals(intervals, NUM_INTERVALS, result);

    printf("merged intervals: ");
    for (i = 0; i < count; i++)
    {
        printf("[%d,%d] ", result[i].start, result[i].end);
    }
    printf("\n");

    return 0;
}
