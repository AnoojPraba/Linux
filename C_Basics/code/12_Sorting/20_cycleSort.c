// Cycle sort: places each value directly at its final position, so it performs
// the minimum number of memory writes.

#include <stdio.h>

static void show(const char *label, const int *a, int n)
{
    printf("%-14s:", label);
    for (int i = 0; i < n; i++) printf(" %d", a[i]);
    printf("\n");
}

int main(void)
{
    int a[] = {20, 40, 50, 10, 30, 10, 50}, n = 7, writes = 0;
    for (int start = 0; start < n - 1; start++)
    {
        int item = a[start], pos = start;
        for (int i = start + 1; i < n; i++)
            if (a[i] < item) pos++;                        /* the number of smaller elements = the final position */
        if (pos == start) continue;                        /* already in place: no write */
        while (item == a[pos]) pos++;                      /* skip duplicates */
        int t = a[pos]; a[pos] = item; item = t; writes++;
        while (pos != start)                               /* rotate the rest of the cycle */
        {
            pos = start;
            for (int i = start + 1; i < n; i++)
                if (a[i] < item) pos++;
            while (item == a[pos]) pos++;
            t = a[pos]; a[pos] = item; item = t; writes++;
        }
    }
    show("cycle sorted", a, n);
    printf("memory writes: %d (selection sort would use up to %d swaps)\n", writes, n - 1);
    return 0;
}
