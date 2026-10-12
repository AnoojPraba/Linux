// Minimum meeting rooms / railway platforms: sort start and end times and
// sweep, or use a min-heap of end times.

#include <stdio.h>
#include <stdlib.h>

static int cmp(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }

int main(void)
{
    int arr[] = {900, 940, 950, 1100, 1500, 1800};      /* arrival times  */
    int dep[] = {910, 1200, 1120, 1130, 1900, 2000};    /* departure times */
    int n = 6;
    qsort(arr, n, sizeof arr[0], cmp);                  /* sorted independently: only the counts matter */
    qsort(dep, n, sizeof dep[0], cmp);

    int platforms = 0, best = 0, i = 0, j = 0;
    while (i < n)
    {
        if (arr[i] <= dep[j]) { platforms++; i++; }     /* a train arrives before the earliest departure */
        else                  { platforms--; j++; }     /* a train leaves */
        if (platforms > best) best = platforms;
    }
    printf("minimum platforms needed = %d\n", best);

    int start[] = {0, 5, 15}, end[] = {30, 10, 20};      /* meeting intervals */
    qsort(start, 3, sizeof start[0], cmp);
    qsort(end, 3, sizeof end[0], cmp);
    int rooms = 0, max_rooms = 0;
    for (int a = 0, b = 0; a < 3;)
        if (start[a] < end[b]) { rooms++; a++; if (rooms > max_rooms) max_rooms = rooms; }
        else { rooms--; b++; }
    printf("meeting rooms needed for (0,30) (5,10) (15,20) = %d\n", max_rooms);
    return 0;
}
