// Linear-search improvements: sentinel search, move-to-front and transpose
// heuristics for skewed access patterns.

#include <stdio.h>
#include <string.h>

static long cost;

static int sentinel_search(int *a, int n, int key)         /* a has room for n + 1 elements */
{
    int last = a[n - 1];
    a[n - 1] = key;                                        /* the sentinel removes the bounds check from the loop */
    int i = 0;
    while (a[i] != key) i++;
    a[n - 1] = last;
    return (i < n - 1 || last == key) ? i : -1;
}

static int mtf_search(int *a, int n, int key)              /* move-to-front: recently used items become cheap */
{
    for (int i = 0; i < n; i++)
    {
        cost++;
        if (a[i] == key)
        {
            memmove(a + 1, a, (size_t)i * sizeof *a);
            a[0] = key;
            return 0;
        }
    }
    return -1;
}

static int transpose_search(int *a, int n, int key)        /* swap one step toward the front */
{
    for (int i = 0; i < n; i++)
    {
        cost++;
        if (a[i] == key)
        {
            if (i > 0) { int t = a[i]; a[i] = a[i - 1]; a[i - 1] = t; }
            return i;
        }
    }
    return -1;
}

int main(void)
{
    int a[] = {5, 9, 2, 7, 4};
    printf("sentinel search: 7 at %d, 8 at %d\n", sentinel_search(a, 5, 7), sentinel_search(a, 5, 8));

    int queries[] = {7, 7, 3, 7, 7, 3, 7, 7, 7, 3};         /* a skewed workload: 7 and 3 dominate */
    int plain[8] = {1, 2, 3, 4, 5, 6, 7, 8}, mtf[8] = {1, 2, 3, 4, 5, 6, 7, 8}, tr[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    long plain_cost = 0;
    for (int q = 0; q < 10; q++)
        for (int i = 0; i < 8; i++) { plain_cost++; if (plain[i] == queries[q]) break; }
    cost = 0;
    for (int q = 0; q < 10; q++) mtf_search(mtf, 8, queries[q]);
    long mtf_cost = cost;
    cost = 0;
    for (int q = 0; q < 10; q++) transpose_search(tr, 8, queries[q]);
    printf("comparisons for 10 skewed queries: plain %ld, move-to-front %ld, transpose %ld\n", plain_cost, mtf_cost, cost);
    printf("move-to-front order afterwards: %d %d %d ...\n", mtf[0], mtf[1], mtf[2]);
    return 0;
}
