// Two more MST algorithms: Prim with a binary heap (O(E log V)) and Boruvka
// (merge the cheapest edge of every component per round).

#include <stdio.h>
#include <string.h>

#define N 7
typedef struct { int u, v, w; } Edge;
static Edge e[] = {{0, 1, 7}, {0, 3, 5}, {1, 2, 8}, {1, 3, 9}, {1, 4, 7}, {2, 4, 5}, {3, 4, 15}, {3, 5, 6}, {4, 5, 8}, {4, 6, 9}, {5, 6, 11}};
#define M ((int)(sizeof e / sizeof e[0]))

/* ---- Prim with a heap of (weight, vertex) ---- */
typedef struct { int w, v; } Item;
static Item heap[64];
static int hn;
static void push(Item x)
{
    int i = hn++;
    heap[i] = x;
    while (i && heap[(i - 1) / 2].w > heap[i].w) { Item t = heap[i]; heap[i] = heap[(i - 1) / 2]; heap[(i - 1) / 2] = t; i = (i - 1) / 2; }
}
static Item pop(void)
{
    Item top = heap[0];
    heap[0] = heap[--hn];
    for (int i = 0;;)
    {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < hn && heap[l].w < heap[m].w) m = l;
        if (r < hn && heap[r].w < heap[m].w) m = r;
        if (m == i) break;
        Item t = heap[i]; heap[i] = heap[m]; heap[m] = t;
        i = m;
    }
    return top;
}

static int prim(void)
{
    int in_tree[N] = {0}, total = 0, taken = 0;
    push((Item){0, 0});
    while (hn && taken < N)
    {
        Item it = pop();
        if (in_tree[it.v]) continue;                   /* lazy deletion of outdated heap entries */
        in_tree[it.v] = 1;
        total += it.w;
        taken++;
        for (int i = 0; i < M; i++)
        {
            if (e[i].u == it.v && !in_tree[e[i].v]) push((Item){e[i].w, e[i].v});
            if (e[i].v == it.v && !in_tree[e[i].u]) push((Item){e[i].w, e[i].u});
        }
    }
    return total;
}

/* ---- Boruvka ---- */
static int parent[N];
static int find(int x) { while (parent[x] != x) x = parent[x] = parent[parent[x]]; return x; }

static int boruvka(void)
{
    int total = 0, comps = N;
    for (int i = 0; i < N; i++) parent[i] = i;
    while (comps > 1)
    {
        int cheapest[N];
        for (int i = 0; i < N; i++) cheapest[i] = -1;
        for (int i = 0; i < M; i++)
        {
            int a = find(e[i].u), b = find(e[i].v);
            if (a == b) continue;
            if (cheapest[a] < 0 || e[i].w < e[cheapest[a]].w) cheapest[a] = i;
            if (cheapest[b] < 0 || e[i].w < e[cheapest[b]].w) cheapest[b] = i;
        }
        for (int i = 0; i < N; i++)
            if (cheapest[i] >= 0)
            {
                int a = find(e[cheapest[i]].u), b = find(e[cheapest[i]].v);
                if (a != b)
                {
                    parent[a] = b;
                    total += e[cheapest[i]].w;
                    comps--;
                }
            }
    }
    return total;
}

int main(void)
{
    printf("Prim (heap)  : MST weight %d\n", prim());
    printf("Boruvka      : MST weight %d\n", boruvka());
    return 0;
}
