// Reservoir sampling and randomized quickselect with a deterministic
// generator.

#include <stdio.h>

/* small deterministic generator so the output is repeatable (replace with a real RNG in practice) */
static unsigned long long state = 12345;
static unsigned next_rand(void)
{
    state = state * 6364136223846793005ULL + 1442695040888963407ULL;
    return (unsigned)(state >> 33);
}

/* Reservoir sampling: pick k items uniformly from a stream of unknown length in one pass. */
static void reservoir(int k, int n_stream, int *out)
{
    for (int i = 0; i < n_stream; i++)
    {
        if (i < k)
            out[i] = i;                         /* fill the reservoir with the first k items */
        else
        {
            unsigned j = next_rand() % (unsigned)(i + 1);
            if (j < (unsigned)k)
                out[j] = i;                     /* keep item i with probability k/(i+1) */
        }
    }
}

/* Quickselect with a random pivot: k-th smallest (0-based) in expected O(n). */
static int quickselect(int *a, int n, int k)
{
    int lo = 0, hi = n - 1;
    while (lo < hi)
    {
        int p = lo + (int)(next_rand() % (unsigned)(hi - lo + 1));   /* random pivot defeats bad inputs */
        int pivot = a[p], t;
        t = a[p]; a[p] = a[hi]; a[hi] = t;
        int store = lo;
        for (int i = lo; i < hi; i++)
            if (a[i] < pivot)
            {
                t = a[i]; a[i] = a[store]; a[store] = t;
                store++;
            }
        t = a[store]; a[store] = a[hi]; a[hi] = t;
        if (k == store) return a[k];
        if (k < store) hi = store - 1; else lo = store + 1;
    }
    return a[lo];
}

int main(void)
{
    int sample[3];
    reservoir(3, 1000, sample);
    int ok = 1;
    for (int i = 0; i < 3; i++)
        if (sample[i] < 0 || sample[i] >= 1000)
            ok = 0;
    printf("3 items from a stream of 1000 chosen, all valid: %s\n", ok ? "yes" : "no");

    int a[] = {9, 1, 8, 2, 7, 3, 6, 4, 5, 0};
    printf("median-ish (5th smallest, index 4) = %d\n", quickselect(a, 10, 4));
    return 0;
}
