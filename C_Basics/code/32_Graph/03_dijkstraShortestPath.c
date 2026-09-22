#include <stdio.h>
#include <limits.h>

#define MAX_VERTICES 6
#define NO_EDGE 0

int graph[MAX_VERTICES][MAX_VERTICES] = {
    {0, 4, 1, 0, 0, 0},
    {4, 0, 2, 5, 0, 0},
    {1, 2, 0, 8, 10, 0},
    {0, 5, 8, 0, 2, 6},
    {0, 0, 10, 2, 0, 3},
    {0, 0, 0, 6, 3, 0}
};

// Pick the unvisited vertex with the smallest known distance so far - the
// same "always take the cheapest next option" idea as a min-heap pop in
// 31_Heap/01_minHeap.c; a real implementation would use that heap instead
// of this O(V) scan for better performance on large graphs.
int minDistanceVertex(int distance[], int visited[])
{
    int minValue = INT_MAX;
    int minIndex = -1;
    int i;

    for (i = 0; i < MAX_VERTICES; i++)
    {
        if ((!visited[i]) && (distance[i] <= minValue))
        {
            minValue = distance[i];
            minIndex = i;
        }
    }
    return minIndex;
}

// Dijkstra's algorithm: repeatedly finalize the closest remaining vertex,
// then relax (potentially shorten) its neighbors' distances through it.
// Only works correctly with non-negative edge weights.
void dijkstra(int source)
{
    int distance[MAX_VERTICES];
    int visited[MAX_VERTICES] = {0};
    int i;
    int count;

    for (i = 0; i < MAX_VERTICES; i++)
    {
        distance[i] = INT_MAX;
    }
    distance[source] = 0;

    for (count = 0; count < MAX_VERTICES - 1; count++)
    {
        int u = minDistanceVertex(distance, visited);
        int v;

        visited[u] = 1;

        for (v = 0; v < MAX_VERTICES; v++)
        {
            int edgeWeight = graph[u][v];

            if ((edgeWeight != NO_EDGE) && (!visited[v]) &&
                (distance[u] != INT_MAX) &&
                (distance[u] + edgeWeight < distance[v]))
            {
                distance[v] = distance[u] + edgeWeight;
            }
        }
    }

    printf("shortest distances from vertex %d:\n", source);
    for (i = 0; i < MAX_VERTICES; i++)
    {
        printf("  to %d: %d\n", i, distance[i]);
    }
}

int main()
{
    dijkstra(0);
    return 0;
}
