// Introsort: quick sort that falls back to heap sort when the recursion gets
// too deep and finishes small ranges with insertion sort (what std::sort
// does).

#include <math.h>
#include <stdio.h>

static void show(const char *label, const int *a, int n)
{
    printf("%-14s:", label);
    for (int i = 0; i < n; i++) printf(" %d", a[i]);
    printf("\n");
}

static long comparisons;
static void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

static void sift_down(int *a, int lo, int n, int i)
{
    for (;;)
    {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < n && (comparisons++, a[lo + l] > a[lo + m])) m = l;
        if (r < n && (comparisons++, a[lo + r] > a[lo + m])) m = r;
        if (m == i) return;
        swap(&a[lo + i], &a[lo + m]);
        i = m;
    }
}

static void heap_sort(int *a, int lo, int hi)             /* sorts a[lo..hi) */
{
    int n = hi - lo;
    for (int i = n / 2 - 1; i >= 0; i--) sift_down(a, lo, n, i);
    for (int end = n - 1; end > 0; end--) { swap(&a[lo], &a[lo + end]); sift_down(a, lo, end, 0); }
}

static void insertion(int *a, int lo, int hi)
{
    for (int i = lo + 1; i < hi; i++)
    {
        int v = a[i], j = i - 1;
        while (j >= lo && (comparisons++, a[j] > v)) { a[j + 1] = a[j]; j--; }
        a[j + 1] = v;
    }
}

static int heap_fallbacks;

static void intro(int *a, int lo, int hi, int depth)
{
    while (hi - lo > 8)                                    /* small ranges are left for insertion sort */
    {
        if (depth-- == 0)
        {
            heap_fallbacks++;
            heap_sort(a, lo, hi);                          /* too deep: guarantee O(n log n) */
            return;
        }
        int mid = lo + (hi - lo) / 2;
        if (a[mid] < a[lo]) swap(&a[mid], &a[lo]);         /* median of three as the pivot */
        if (a[hi - 1] < a[lo]) swap(&a[hi - 1], &a[lo]);
        if (a[hi - 1] < a[mid]) swap(&a[hi - 1], &a[mid]);
        int pivot = a[mid], i = lo, j = hi - 1;
        while (i <= j)
        {
            while (comparisons++, a[i] < pivot) i++;
            while (comparisons++, a[j] > pivot) j--;
            if (i <= j) swap(&a[i++], &a[j--]);
        }
        intro(a, lo, j + 1, depth);
        lo = i;                                            /* loop on the right part: bounded stack */
    }
    insertion(a, lo, hi);
}

int main(void)
{
    int a[200];
    for (int i = 0; i < 200; i++) a[i] = (i * 7919) % 211;     /* a scrambled permutation-like sequence */
    int depth_limit = 2 * (int)log2(200);
    intro(a, 0, 200, depth_limit);
    int ok = 1;
    for (int i = 1; i < 200; i++) if (a[i - 1] > a[i]) ok = 0;
    printf("sorted: %s, comparisons: %ld, heap-sort fallbacks: %d (depth limit %d)\n", ok ? "yes" : "no", comparisons, heap_fallbacks, depth_limit);
    int b[200];
    for (int i = 0; i < 200; i++) b[i] = (i * 7919) % 211;
    heap_fallbacks = 0;
    intro(b, 0, 200, 1);                                   /* an absurdly small depth limit forces the heap-sort fallback */
    ok = 1;
    for (int i = 1; i < 200; i++) if (b[i - 1] > b[i]) ok = 0;
    printf("with depth limit 1: sorted: %s, heap-sort fallbacks: %d\n", ok ? "yes" : "no", heap_fallbacks);
    int small[] = {5, 2, 9, 1, 7};
    intro(small, 0, 5, depth_limit);
    show("small range", small, 5);
    return 0;
}
