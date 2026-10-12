// Binary min-heap picking the shortest burst first.

#include <stdio.h>

typedef struct
{
    int burst;                      /* the key: smaller runs first */
    const char *name;
} Task;

static Task heap[16];
static int n;

static void swap(Task *a, Task *b)
{
    Task t = *a;
    *a = *b;
    *b = t;
}

static void push(Task t)
{
    int i = n++;
    heap[i] = t;
    while (i > 0 && heap[(i - 1) / 2].burst > heap[i].burst)   /* sift up */
    {
        swap(&heap[(i - 1) / 2], &heap[i]);
        i = (i - 1) / 2;
    }
}

static Task pop(void)
{
    Task top = heap[0];
    heap[0] = heap[--n];
    int i = 0;
    for (;;)                                                    /* sift down */
    {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < n && heap[l].burst < heap[m].burst) m = l;
        if (r < n && heap[r].burst < heap[m].burst) m = r;
        if (m == i) break;
        swap(&heap[i], &heap[m]);
        i = m;
    }
    return top;
}

int main(void)
{
    push((Task){8, "P1"});
    push((Task){4, "P2"});
    push((Task){9, "P3"});
    push((Task){5, "P4"});
    push((Task){2, "P5"});

    int clock = 0;
    printf("shortest job first (all arrive at time 0):\n");
    while (n > 0)
    {
        Task t = pop();
        printf("  time %2d: run %s for %d\n", clock, t.name, t.burst);
        clock += t.burst;
    }
    return 0;
}
