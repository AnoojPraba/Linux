// Race of sorting algorithms on the same data: counted comparisons for
// insertion, selection, bubble, merge, quick and heap sort on random, sorted
// and reversed input.

#include <stdio.h>
#include <string.h>

#define N 200
static long cmp_count;

static int less(int a, int b) { cmp_count++; return a < b; }
static void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

static void insertion(int *a, int n)
{
    for (int i = 1; i < n; i++)
    {
        int v = a[i], j = i - 1;
        while (j >= 0 && less(v, a[j])) { a[j + 1] = a[j]; j--; }
        a[j + 1] = v;
    }
}

static void selection(int *a, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int m = i;
        for (int j = i + 1; j < n; j++) if (less(a[j], a[m])) m = j;
        swap(&a[i], &a[m]);
    }
}

static void bubble(int *a, int n)
{
    for (int pass = 0, sw = 1; sw; pass++)
    {
        sw = 0;
        for (int i = 0; i + 1 < n - pass; i++)
            if (less(a[i + 1], a[i])) { swap(&a[i], &a[i + 1]); sw = 1; }
    }
}

static void merge_sort(int *a, int *t, int lo, int hi)
{
    if (hi - lo < 2) return;
    int mid = (lo + hi) / 2, i = lo, j = mid, k = lo;
    merge_sort(a, t, lo, mid);
    merge_sort(a, t, mid, hi);
    while (i < mid && j < hi) t[k++] = less(a[j], a[i]) ? a[j++] : a[i++];
    while (i < mid) t[k++] = a[i++];
    while (j < hi) t[k++] = a[j++];
    memcpy(a + lo, t + lo, (size_t)(hi - lo) * sizeof(int));
}

static void quick(int *a, int lo, int hi)
{
    if (lo >= hi) return;
    swap(&a[(lo + hi) / 2], &a[hi]);
    int p = a[hi], i = lo;
    for (int j = lo; j < hi; j++) if (less(a[j], p)) swap(&a[i++], &a[j]);
    swap(&a[i], &a[hi]);
    quick(a, lo, i - 1);
    quick(a, i + 1, hi);
}

static void sift(int *a, int n, int i)
{
    for (;;)
    {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < n && less(a[m], a[l])) m = l;
        if (r < n && less(a[m], a[r])) m = r;
        if (m == i) return;
        swap(&a[i], &a[m]);
        i = m;
    }
}

static void heap_sort(int *a, int n)
{
    for (int i = n / 2 - 1; i >= 0; i--) sift(a, n, i);
    for (int e = n - 1; e > 0; e--) { swap(&a[0], &a[e]); sift(a, e, 0); }
}

static void run(const char *name, int kind, const int *base, const char *shape)
{
    int a[N], t[N];
    memcpy(a, base, sizeof a);
    cmp_count = 0;
    switch (kind)
    {
    case 0: insertion(a, N); break;
    case 1: selection(a, N); break;
    case 2: bubble(a, N); break;
    case 3: merge_sort(a, t, 0, N); break;
    case 4: quick(a, 0, N - 1); break;
    default: heap_sort(a, N);
    }
    printf("  %-10s on %-8s: %6ld comparisons\n", name, shape, cmp_count);
}

int main(void)
{
    int random_[N], sorted[N], reversed[N];
    unsigned s = 7;
    for (int i = 0; i < N; i++) { s = s * 1103515245u + 12345u; random_[i] = (int)(s >> 16) % 1000; sorted[i] = i; reversed[i] = N - i; }
    const char *names[] = {"insertion", "selection", "bubble", "merge", "quick", "heap"};
    const int *inputs[] = {random_, sorted, reversed};
    const char *shapes[] = {"random", "sorted", "reversed"};
    for (int in = 0; in < 3; in++)
    {
        printf("n = %d, %s input:\n", N, shapes[in]);
        for (int k = 0; k < 6; k++) run(names[k], k, inputs[in], shapes[in]);
    }
    return 0;
}
