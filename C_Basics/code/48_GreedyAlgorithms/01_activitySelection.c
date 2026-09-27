#include <stdio.h>

#define NUM_ACTIVITIES 6

/*****************************************************************************
 * Name: sortByEndTime
 *
 * Description:
 *         Simple insertion sort of the activities by end time ascending.
 *
 * Inputs:
 *         start : array of activity start times, sorted in lockstep.
 *         end   : array of activity end times, sorted in lockstep.
 *         n     : number of activities.
 *****************************************************************************/
void sortByEndTime(int start[], int end[], int n)
{
    int i;
    int j;

    for (i = 1; i < n; i++)
    {
        int startKey = start[i];
        int endKey = end[i];

        j = i - 1;

        while ((j >= 0) && (end[j] > endKey))
        {
            start[j + 1] = start[j];
            end[j + 1] = end[j];
            j--;
        }

        start[j + 1] = startKey;
        end[j + 1] = endKey;
    }
}

/*****************************************************************************
 * Name: selectMaxActivities
 *
 * Description:
 *         Greedy activity selection: sort activities by end time, then
 *         always pick the next activity whose start time is not before the
 *         previously selected activity's end time. Picking the earliest-
 *         ending compatible activity first is optimal - exchange-argument
 *         intuition: if an optimal solution picked some other activity
 *         first, swapping it for the earliest-ending one can only free up
 *         more room for later choices, never less, so the swap never makes
 *         the solution worse. Repeating this argument for every pick shows
 *         earliest-end-time-first loses nothing versus any optimal schedule.
 *
 * Inputs:
 *         start : array of activity start times.
 *         end   : array of activity end times, sorted ascending.
 *         n     : number of activities.
 *
 * Returns:
 *         The maximum number of non-overlapping activities selectable.
 *****************************************************************************/
int selectMaxActivities(int start[], int end[], int n)
{
    int count;
    int lastEnd;
    int i;

    count = 1;
    lastEnd = end[0];

    printf("selected: [%d, %d] ", start[0], end[0]);

    for (i = 1; i < n; i++)
    {
        if (start[i] >= lastEnd)
        {
            printf("[%d, %d] ", start[i], end[i]);
            lastEnd = end[i];
            count++;
        }
    }

    printf("\n");

    return count;
}

int main()
{
    int start[NUM_ACTIVITIES] = {1, 3, 0, 5, 8, 5};
    int end[NUM_ACTIVITIES] = {2, 4, 6, 7, 9, 9};

    sortByEndTime(start, end, NUM_ACTIVITIES);

    printf("max non-overlapping activities = %d\n",
           selectMaxActivities(start, end, NUM_ACTIVITIES));

    return 0;
}
