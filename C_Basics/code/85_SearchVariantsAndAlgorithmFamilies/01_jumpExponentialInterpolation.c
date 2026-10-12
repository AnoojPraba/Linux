// Jump (O(sqrt n)), exponential/galloping (O(log i)) and interpolation (O(log
// log n) for uniform data) search compared by probe count.

#include <math.h>
#include <stdio.h>

static int steps;

static int binary_range(const int *a, int lo, int hi, int key)   /* a[lo..hi] inclusive */
{
    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;
        steps++;
        if (a[mid] == key) return mid;
        if (a[mid] < key) lo = mid + 1; else hi = mid - 1;
    }
    return -1;
}

static int jump_search(const int *a, int n, int key)
{
    int step = (int)sqrt((double)n), prev = 0;
    while (prev < n && a[(step < n ? step : n) - 1] < key)       /* jump ahead block by block */
    {
        steps++;
        prev = step;
        step += (int)sqrt((double)n);
        if (prev >= n) return -1;
    }
    for (int i = prev; i < (step < n ? step : n); i++)           /* linear scan inside the block */
    {
        steps++;
        if (a[i] == key) return i;
    }
    return -1;
}

static int exponential_search(const int *a, int n, int key)
{
    if (n > 0 && a[0] == key) { steps++; return 0; }
    int bound = 1;
    while (bound < n && a[bound] <= key)                         /* double the range */
    {
        steps++;
        bound *= 2;
    }
    return binary_range(a, bound / 2, (bound < n ? bound : n) - 1, key);
}

static int interpolation_search(const int *a, int n, int key)
{
    int lo = 0, hi = n - 1;
    while (lo <= hi && key >= a[lo] && key <= a[hi])
    {
        steps++;
        if (a[hi] == a[lo])
            return a[lo] == key ? lo : -1;
        int pos = lo + (int)((double)(key - a[lo]) * (hi - lo) / (a[hi] - a[lo]));  /* guess by value */
        if (a[pos] == key) return pos;
        if (a[pos] < key) lo = pos + 1; else hi = pos - 1;
    }
    return -1;
}

int main(void)
{
    int a[100];
    for (int i = 0; i < 100; i++)
        a[i] = 10 + i * 3;                      /* sorted, evenly spread: 10, 13, ..., 307 */

    int key = 250;                              /* index 80 */
    steps = 0; int r1 = binary_range(a, 0, 99, key);        printf("binary        : index %d in %d probes\n", r1, steps);
    steps = 0; int r2 = jump_search(a, 100, key);           printf("jump          : index %d in %d probes\n", r2, steps);
    steps = 0; int r3 = exponential_search(a, 100, key);    printf("exponential   : index %d in %d probes\n", r3, steps);
    steps = 0; int r4 = interpolation_search(a, 100, key);  printf("interpolation : index %d in %d probes\n", r4, steps);
    steps = 0; int r5 = jump_search(a, 100, 251);           printf("jump, missing : index %d\n", r5);
    return 0;
}
