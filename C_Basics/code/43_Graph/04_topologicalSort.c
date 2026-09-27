#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 6

typedef struct AdjNode
{
    int vertex;
    struct AdjNode *next;
} AdjNode;

typedef struct
{
    AdjNode *adjacencyList[MAX_VERTICES];
    int numVertices;
} Graph;

void initGraph(Graph *g, int numVertices)
{
    int i;

    g->numVertices = numVertices;
    for (i = 0; i < numVertices; i++)
    {
        g->adjacencyList[i] = NULL;
    }
}

// Directed edge only (unlike the undirected addEdge in 01_adjacencyListBFS.c):
// a dependency graph's edges point from prerequisite to dependent.
void addEdge(Graph *g, int src, int dest)
{
    AdjNode *node = malloc(sizeof(AdjNode));

    node->vertex = dest;
    node->next = g->adjacencyList[src];
    g->adjacencyList[src] = node;
}

void freeGraph(Graph *g)
{
    int i;

    for (i = 0; i < g->numVertices; i++)
    {
        AdjNode *current = g->adjacencyList[i];

        while (current != NULL)
        {
            AdjNode *next = current->next;

            free(current);
            current = next;
        }
    }
}

// Kahn's algorithm: repeatedly peel off vertices with no remaining incoming
// edges. This only works on a DAG - if the graph has a cycle, every vertex
// in that cycle always has at least one unpeeled incoming edge, so fewer
// than numVertices vertices ever get processed and no full ordering exists.
void topologicalSort(Graph *g)
{
    int inDegree[MAX_VERTICES] = {0};
    int queue[MAX_VERTICES];
    int order[MAX_VERTICES];
    int front = 0;
    int back = 0;
    int processed = 0;
    int i;

    for (i = 0; i < g->numVertices; i++)
    {
        AdjNode *neighbor = g->adjacencyList[i];

        while (neighbor != NULL)
        {
            inDegree[neighbor->vertex]++;
            neighbor = neighbor->next;
        }
    }

    for (i = 0; i < g->numVertices; i++)
    {
        if (inDegree[i] == 0)
        {
            queue[back++] = i;
        }
    }

    while (front < back)
    {
        int current = queue[front++];
        AdjNode *neighbor = g->adjacencyList[current];

        order[processed++] = current;
        while (neighbor != NULL)
        {
            inDegree[neighbor->vertex]--;
            if (inDegree[neighbor->vertex] == 0)
            {
                queue[back++] = neighbor->vertex;
            }
            neighbor = neighbor->next;
        }
    }

    if (processed < g->numVertices)
    {
        printf("cycle detected: only %d of %d vertices could be ordered\n",
               processed, g->numVertices);
        return;
    }

    printf("topological order: ");
    for (i = 0; i < processed; i++)
    {
        printf("%d ", order[i]);
    }
    printf("\n");
}

int main(void)
{
    Graph g;

    initGraph(&g, 6);
    addEdge(&g, 5, 2);
    addEdge(&g, 5, 0);
    addEdge(&g, 4, 0);
    addEdge(&g, 4, 1);
    addEdge(&g, 2, 3);
    addEdge(&g, 3, 1);

    topologicalSort(&g);

    freeGraph(&g);
    return 0;
}
