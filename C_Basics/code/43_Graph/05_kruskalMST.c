#include <stdio.h>
#include <stdlib.h>

#define NUM_VERTICES 6
#define NUM_EDGES 9

typedef struct
{
    int u;
    int v;
    int weight;
} Edge;

// Union-find with union-by-rank and path compression, mirroring
// 42_UnionFind/02_unionFindOptimized.c - used here to detect whether an
// edge's endpoints are already connected, i.e. whether adding it forms
// a cycle.
int find(int *parent, int element)
{
    if (parent[element] != element)
    {
        parent[element] = find(parent, parent[element]);
    }
    return parent[element];
}

void unionSets(int *parent, int *rank, int a, int b)
{
    int rootA = find(parent, a);
    int rootB = find(parent, b);

    if (rootA == rootB)
    {
        return;
    }

    if (rank[rootA] < rank[rootB])
    {
        parent[rootA] = rootB;
    }
    else if (rank[rootA] > rank[rootB])
    {
        parent[rootB] = rootA;
    }
    else
    {
        parent[rootB] = rootA;
        rank[rootA] = rank[rootA] + 1;
    }
}

int compareEdges(const void *a, const void *b)
{
    const Edge *edgeA = (const Edge *)a;
    const Edge *edgeB = (const Edge *)b;

    return edgeA->weight - edgeB->weight;
}

// Kruskal's MST: sort all edges by weight, then greedily take each edge
// unless its endpoints are already in the same union-find set (which would
// close a cycle instead of growing the tree).
void kruskalMST(Edge edges[], int numEdges, int numVertices)
{
    int parent[NUM_VERTICES];
    int rank[NUM_VERTICES];
    int totalWeight = 0;
    int edgesUsed = 0;
    int i;

    for (i = 0; i < numVertices; i++)
    {
        parent[i] = i;
        rank[i] = 0;
    }

    qsort(edges, numEdges, sizeof(Edge), compareEdges);

    printf("MST edges (Kruskal):\n");
    for (i = 0; ((i < numEdges) && (edgesUsed < numVertices - 1)); i++)
    {
        int rootU = find(parent, edges[i].u);
        int rootV = find(parent, edges[i].v);

        if (rootU != rootV)
        {
            unionSets(parent, rank, rootU, rootV);
            printf("  %d - %d (weight %d)\n", edges[i].u, edges[i].v, edges[i].weight);
            totalWeight += edges[i].weight;
            edgesUsed++;
        }
    }

    printf("total weight: %d\n", totalWeight);
}

int main(void)
{
    Edge edges[NUM_EDGES] = {
        {0, 1, 4},
        {0, 2, 1},
        {1, 2, 2},
        {1, 3, 5},
        {2, 3, 8},
        {2, 4, 10},
        {3, 4, 2},
        {3, 5, 6},
        {4, 5, 3}
    };

    kruskalMST(edges, NUM_EDGES, NUM_VERTICES);

    return 0;
}
