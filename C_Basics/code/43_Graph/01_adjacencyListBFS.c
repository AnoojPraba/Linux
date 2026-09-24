#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 10

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

// Undirected edge: add both directions. Each vertex's adjacency list is a
// singly linked list of the vertices reachable in one hop.
void addEdge(Graph *g, int src, int dest)
{
    AdjNode *node = malloc(sizeof(AdjNode));

    node->vertex = dest;
    node->next = g->adjacencyList[src];
    g->adjacencyList[src] = node;

    node = malloc(sizeof(AdjNode));
    node->vertex = src;
    node->next = g->adjacencyList[dest];
    g->adjacencyList[dest] = node;
}

// Breadth-first search: an explicit queue (like the tree level-order
// traversal in 38_BinaryTree/03_heightAndLevelOrder.c) visits every vertex
// at the current distance before moving further out, and "visited" stops
// a graph's cycles from causing infinite revisits, unlike a tree.
void bfs(Graph *g, int startVertex)
{
    int visited[MAX_VERTICES] = {0};
    int queue[MAX_VERTICES];
    int front = 0;
    int back = 0;

    visited[startVertex] = 1;
    queue[back++] = startVertex;

    printf("BFS from %d: ", startVertex);
    while (front < back)
    {
        int current = queue[front++];
        AdjNode *neighbor = g->adjacencyList[current];

        printf("%d ", current);
        while (neighbor != NULL)
        {
            if (!visited[neighbor->vertex])
            {
                visited[neighbor->vertex] = 1;
                queue[back++] = neighbor->vertex;
            }
            neighbor = neighbor->next;
        }
    }
    printf("\n");
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

int main()
{
    Graph g;

    initGraph(&g, 6);
    addEdge(&g, 0, 1);
    addEdge(&g, 0, 2);
    addEdge(&g, 1, 3);
    addEdge(&g, 2, 4);
    addEdge(&g, 3, 5);
    addEdge(&g, 4, 5);

    bfs(&g, 0);

    freeGraph(&g);
    return 0;
}
