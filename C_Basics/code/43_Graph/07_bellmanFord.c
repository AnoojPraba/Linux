#include <stdio.h>
#include <limits.h>

#define NUM_VERTICES 5
#define NUM_EDGES 8

typedef struct
{
    int u;
    int v;
    int weight;
} Edge;

// Bellman-Ford: relax every edge V-1 times. Unlike Dijkstra
// (03_dijkstraShortestPath.c), which greedily finalizes the closest vertex
// and breaks with negative weights, this brute-force relaxation still
// converges correctly as long as there is no negative-weight cycle.
void bellmanFord(Edge edges[], int numEdges, int numVertices, int source)
{
    int distance[NUM_VERTICES];
    int i;
    int pass;
    int hasNegativeCycle = 0;

    for (i = 0; i < numVertices; i++)
    {
        distance[i] = INT_MAX;
    }
    distance[source] = 0;

    for (pass = 0; pass < numVertices - 1; pass++)
    {
        for (i = 0; i < numEdges; i++)
        {
            int u = edges[i].u;
            int v = edges[i].v;
            int weight = edges[i].weight;

            if ((distance[u] != INT_MAX) && (distance[u] + weight < distance[v]))
            {
                distance[v] = distance[u] + weight;
            }
        }
    }

    // One extra pass: if any edge can still be relaxed, a negative-weight
    // cycle is reachable from the source, so shortest paths are undefined.
    for (i = 0; i < numEdges; i++)
    {
        int u = edges[i].u;
        int v = edges[i].v;
        int weight = edges[i].weight;

        if ((distance[u] != INT_MAX) && (distance[u] + weight < distance[v]))
        {
            hasNegativeCycle = 1;
        }
    }

    if (hasNegativeCycle)
    {
        printf("negative-weight cycle detected reachable from vertex %d\n", source);
        return;
    }

    printf("shortest distances from vertex %d:\n", source);
    for (i = 0; i < numVertices; i++)
    {
        printf("  to %d: %d\n", i, distance[i]);
    }
}

int main(void)
{
    // Includes a negative edge weight (4 -> 1) but no negative cycle -
    // exactly the case Dijkstra cannot handle correctly.
    Edge edges[NUM_EDGES] = {
        {0, 1, 6},
        {0, 2, 7},
        {1, 3, 5},
        {1, 4, -4},
        {2, 3, -3},
        {2, 4, 9},
        {3, 1, -2},
        {4, 0, 2}
    };

    bellmanFord(edges, NUM_EDGES, NUM_VERTICES, 0);

    return 0;
}
