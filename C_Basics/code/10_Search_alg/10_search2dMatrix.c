// Searching a 2D matrix: staircase search from the top-right corner (rows and
// columns sorted) and binary search over a fully sorted matrix.

#include <stdio.h>

#define R 4
#define C 5

static const int m[R][C] = {
    {1, 4, 7, 11, 15},
    {2, 5, 8, 12, 19},
    {3, 6, 9, 16, 22},
    {10, 13, 14, 17, 24}};

static int staircase(int key, int *steps)                 /* rows and columns sorted independently: O(R + C) */
{
    int r = 0, c = C - 1;
    *steps = 0;
    while (r < R && c >= 0)
    {
        (*steps)++;
        if (m[r][c] == key) return 1;
        if (m[r][c] > key) c--;                           /* the whole column below is larger: discard it */
        else r++;                                         /* the rest of the row is smaller: discard it */
    }
    return 0;
}

static const int flat[3][4] = {{1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}};

static int sorted_matrix(int key)                         /* each row continues the previous: treat it as one sorted array */
{
    int lo = 0, hi = 3 * 4 - 1;
    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2, v = flat[mid / 4][mid % 4];
        if (v == key) return 1;
        if (v < key) lo = mid + 1; else hi = mid - 1;
    }
    return 0;
}

int main(void)
{
    int keys[] = {5, 20, 14, 30};
    for (int i = 0; i < 4; i++)
    {
        int steps;
        int found = staircase(keys[i], &steps);
        printf("staircase search %2d: %s in %d steps\n", keys[i], found ? "found" : "missing", steps);
    }
    printf("fully sorted matrix: 16 -> %d, 13 -> %d\n", sorted_matrix(16), sorted_matrix(13));
    return 0;
}
