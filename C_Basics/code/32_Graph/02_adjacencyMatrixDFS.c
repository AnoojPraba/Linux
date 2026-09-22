#include <stdio.h>

#define MAX_VERTICES 6

typedef struct
{
    int matrix[MAX_VERTICES][MAX_VERTICES];
    int numVertices;
} Graph;

void initGraph(Graph *g, int numVertices)
{
    int i;
    int j;

    g->numVertices = numVertices;
    for (i = 0; i < numVertices; i++)
    {
        for (j = 0; j < numVertices; j++)
        {
            g->matrix[i][j] = 0;
        }
    }
}

// An adjacency matrix trades memory (always numVertices^2, regardless of
// how many edges actually exist) for an O(1) "is there an edge" check,
// unlike the adjacency list in 01_adjacencyListBFS.c which is O(degree)
// to check but only uses memory proportional to the actual edge count.
void addEdge(Graph *g, int src, int dest)
{
    g->matrix[src][dest] = 1;
    g->matrix[dest][src] = 1;
}

// Depth-first search: recurse into the first unvisited neighbor found,
// going as deep as possible before backtracking - the call stack plays
// the same role here that the explicit queue plays for BFS.
void dfs(Graph *g, int vertex, int visited[])
{
    int i;

    visited[vertex] = 1;
    printf("%d ", vertex);

    for (i = 0; i < g->numVertices; i++)
    {
        if ((g->matrix[vertex][i]) && (!visited[i]))
        {
            dfs(g, i, visited);
        }
    }
}

int main()
{
    Graph g;
    int visited[MAX_VERTICES] = {0};

    initGraph(&g, 6);
    addEdge(&g, 0, 1);
    addEdge(&g, 0, 2);
    addEdge(&g, 1, 3);
    addEdge(&g, 2, 4);
    addEdge(&g, 3, 5);
    addEdge(&g, 4, 5);

    printf("DFS from 0: ");
    dfs(&g, 0, visited);
    printf("\n");

    return 0;
}
