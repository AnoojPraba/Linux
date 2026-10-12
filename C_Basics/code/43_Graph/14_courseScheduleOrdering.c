// Course-schedule style problems: is there a valid order, one order, and the
// lexicographically smallest order with a min-heap.

#include <stdio.h>
#include <string.h>

#define MAXN 8
static int pre[MAXN][MAXN];           /* pre[a][b] = 1: take a before b */

static int heap[MAXN], hn;
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

static int schedule(int n, int *out)
{
    int indeg[MAXN] = {0}, k = 0;
    hn = 0;
    for (int a = 0; a < n; a++)
        for (int b = 0; b < n; b++) indeg[b] += pre[a][b];
    for (int i = 0; i < n; i++)
        if (!indeg[i]) push(i);
    while (hn)
    {
        int u = pop();                                 /* the smallest available course number */
        out[k++] = u;
        for (int v = 0; v < n; v++)
            if (pre[u][v] && --indeg[v] == 0) push(v);
    }
    return k == n;                                     /* fewer than n taken: a cycle */
}

int main(void)
{
    int out[MAXN];
    pre[5][2] = pre[5][0] = pre[4][0] = pre[4][1] = pre[2][3] = pre[3][1] = 1;
    printf("can finish all 6 courses: %d, smallest-numbered order:", schedule(6, out));
    for (int i = 0; i < 6; i++) printf(" %d", out[i]);
    printf("\n");
    pre[1][5] = 1;                                     /* 5 -> 2 -> 3 -> 1 -> 5 */
    printf("after adding 1 -> 5: can finish: %d\n", schedule(6, out));
    return 0;
}
