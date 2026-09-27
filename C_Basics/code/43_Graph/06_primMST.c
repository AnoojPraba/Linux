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

// Pick the vertex not yet in the MST with the smallest known connecting
// edge weight - same O(V) scan as minDistanceVertex in
// 03_dijkstraShortestPath.c, just minimizing "cheapest edge into the tree"
// instead of "shortest path from the source".
int minKeyVertex(int key[], int inMST[])
{
    int minValue = INT_MAX;
    int minIndex = -1;
    int i;

    for (i = 0; i < MAX_VERTICES; i++)
    {
        if ((!inMST[i]) && (key[i] <= minValue))
        {
            minValue = key[i];
            minIndex = i;
        }
    }
    return minIndex;
}

// Prim's MST: grow a tree from an arbitrary start vertex, at each step
// adding the cheapest edge that crosses the cut between the tree and the
// rest of the graph. A simple O(V^2) array scan is used here, matching the
// style of Dijkstra's minDistanceVertex rather than a binary-heap version.
void primMST(void)
{
    int key[MAX_VERTICES];
    int parent[MAX_VERTICES];
    int inMST[MAX_VERTICES] = {0};
    int totalWeight = 0;
    int i;
    int count;

    for (i = 0; i < MAX_VERTICES; i++)
    {
        key[i] = INT_MAX;
        parent[i] = -1;
    }
    key[0] = 0;

    for (count = 0; count < MAX_VERTICES; count++)
    {
        int u = minKeyVertex(key, inMST);
        int v;

        inMST[u] = 1;

        for (v = 0; v < MAX_VERTICES; v++)
        {
            int edgeWeight = graph[u][v];

            if ((edgeWeight != NO_EDGE) && (!inMST[v]) && (edgeWeight < key[v]))
            {
                key[v] = edgeWeight;
                parent[v] = u;
            }
        }
    }

    printf("MST edges (Prim):\n");
    for (i = 1; i < MAX_VERTICES; i++)
    {
        printf("  %d - %d (weight %d)\n", parent[i], i, key[i]);
        totalWeight += key[i];
    }
    printf("total weight: %d\n", totalWeight);
}

int main(void)
{
    primMST();
    return 0;
}
