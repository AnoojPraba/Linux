// Optimal merge pattern: merge sorted files in the cheapest order by
// repeatedly merging the two smallest (same greedy as Huffman), also
// connecting ropes.

#include <stdio.h>

static int heap[32], hn;

static void push(int x)
{
    int i = hn++;
    heap[i] = x;
    while (i && heap[(i - 1) / 2] > heap[i]) { int t = heap[i]; heap[i] = heap[(i - 1) / 2]; heap[(i - 1) / 2] = t; i = (i - 1) / 2; }
}

static int pop(void)
{
    int top = heap[0];
    heap[0] = heap[--hn];
    for (int i = 0;;)
    {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < hn && heap[l] < heap[m]) m = l;
        if (r < hn && heap[r] < heap[m]) m = r;
        if (m == i) break;
        int t = heap[i]; heap[i] = heap[m]; heap[m] = t;
        i = m;
    }
    return top;
}

static int cheapest_merge(const int *sizes, int n, int verbose)
{
    hn = 0;
    for (int i = 0; i < n; i++) push(sizes[i]);
    int cost = 0;
    while (hn > 1)
    {
        int a = pop(), b = pop();
        if (verbose) printf("  merge %d + %d -> %d\n", a, b, a + b);
        cost += a + b;
        push(a + b);
    }
    return cost;
}

int main(void)
{
    int files[] = {20, 30, 10, 5, 30};
    printf("optimal merge of files 20 30 10 5 30:\n");
    printf("total cost = %d\n", cheapest_merge(files, 5, 1));

    int ropes[] = {4, 3, 2, 6};
    printf("connect ropes 4 3 2 6 with minimum cost = %d\n", cheapest_merge(ropes, 4, 0));

    /* the naive left-to-right order for comparison */
    int naive = 0, acc = files[0];
    for (int i = 1; i < 5; i++) { acc += files[i]; naive += acc; }
    printf("merging in the given order costs %d\n", naive);
    return 0;
}
