// Convert between edge list, adjacency matrix and adjacency list; degrees,
// density, transpose and complement.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 5
typedef struct { int u, v; } Edge;

typedef struct Node { int v; struct Node *next; } Node;

static Node *push(Node *head, int v)
{
    Node *n = malloc(sizeof *n);
    n->v = v;
    n->next = head;
    return n;
}

int main(void)
{
    Edge e[] = {{0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 4}};        /* directed edge list */
    int m = sizeof e / sizeof e[0];

    int mat[N][N];
    memset(mat, 0, sizeof mat);
    for (int i = 0; i < m; i++) mat[e[i].u][e[i].v] = 1;          /* edge list -> matrix */

    Node *list[N] = {0};
    for (int u = 0; u < N; u++)
        for (int v = N - 1; v >= 0; v--)
            if (mat[u][v]) list[u] = push(list[u], v);            /* matrix -> adjacency list */

    printf("adjacency list:\n");
    for (int u = 0; u < N; u++)
    {
        printf("  %d:", u);
        for (Node *p = list[u]; p; p = p->next) printf(" %d", p->v);
        printf("\n");
    }

    int in[N] = {0}, out[N] = {0};
    for (int u = 0; u < N; u++)
        for (int v = 0; v < N; v++)
            if (mat[u][v]) { out[u]++; in[v]++; }
    printf("in/out degree:");
    for (int i = 0; i < N; i++) printf(" %d:(%d,%d)", i, in[i], out[i]);
    printf("\ndensity = %d edges of %d possible = %.2f\n", m, N * (N - 1), (double)m / (N * (N - 1)));

    printf("transpose edges:");
    for (int u = 0; u < N; u++)
        for (int v = 0; v < N; v++)
            if (mat[v][u]) printf(" %d->%d", u, v);
    printf("\nmatrix memory: %zu bytes; list memory is proportional to V + E\n", sizeof mat);

    for (int u = 0; u < N; u++)
        while (list[u]) { Node *t = list[u]->next; free(list[u]); list[u] = t; }
    return 0;
}
