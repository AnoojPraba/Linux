// External sorting idea: sort fixed-size chunks that fit in memory, then k-way
// merge the sorted runs with a min-heap (simulated on arrays).

#include <stdio.h>
#include <stdlib.h>

#define MEM 4                                              /* "memory" holds 4 numbers at a time */

static int cmp(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }

typedef struct { int value, run, idx; } Entry;
static Entry heap[16];
static int hn;

static void push(Entry e)
{
    int i = hn++;
    heap[i] = e;
    while (i && heap[(i - 1) / 2].value > heap[i].value) { Entry t = heap[i]; heap[i] = heap[(i - 1) / 2]; heap[(i - 1) / 2] = t; i = (i - 1) / 2; }
}

static Entry pop(void)
{
    Entry top = heap[0];
    heap[0] = heap[--hn];
    for (int i = 0;;)
    {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < hn && heap[l].value < heap[m].value) m = l;
        if (r < hn && heap[r].value < heap[m].value) m = r;
        if (m == i) break;
        Entry t = heap[i]; heap[i] = heap[m]; heap[m] = t;
        i = m;
    }
    return top;
}

int main(void)
{
    int data[] = {42, 7, 19, 3, 88, 15, 61, 2, 34, 9, 50, 27, 11, 70};   /* pretend this is a huge file */
    int n = 14, runs = (n + MEM - 1) / MEM;
    int run_start[8], run_len[8];

    for (int r = 0; r < runs; r++)                          /* phase 1: sort memory-sized chunks into runs */
    {
        run_start[r] = r * MEM;
        run_len[r] = n - r * MEM < MEM ? n - r * MEM : MEM;
        qsort(data + run_start[r], run_len[r], sizeof(int), cmp);
        printf("run %d:", r);
        for (int i = 0; i < run_len[r]; i++) printf(" %d", data[run_start[r] + i]);
        printf("\n");
    }
    for (int r = 0; r < runs; r++) push((Entry){data[run_start[r]], r, 0});   /* phase 2: one head element per run */
    printf("merged:");
    while (hn)
    {
        Entry e = pop();
        printf(" %d", e.value);
        if (e.idx + 1 < run_len[e.run]) push((Entry){data[run_start[e.run] + e.idx + 1], e.run, e.idx + 1});
    }
    printf("\nmemory needed in phase 2: %d entries (one per run) regardless of file size\n", runs);
    return 0;
}
