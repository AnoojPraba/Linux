#include <stdio.h>

#define ELEMENT_COUNT 10

/*****************************************************************************
 * Name: find
 *
 * Description:
 *         Follows parent pointers to the root of the set containing an
 *         element, flattening the path (path compression) so every visited
 *         node points directly at the root afterward.
 *
 * Inputs:
 *         parent  : parent array.
 *         element : element to find the root of.
 *
 * Returns:
 *         Root of the set containing element.
 *****************************************************************************/
int find(int *parent, int element)
{
    if (parent[element] != element)
    {
        parent[element] = find(parent, parent[element]);
    }
    return parent[element];
}

/*****************************************************************************
 * Name: unionSets
 *
 * Description:
 *         Merges the sets containing two elements, attaching the smaller-
 *         rank root under the larger-rank root (union by rank) to keep
 *         trees shallow.
 *
 * Inputs:
 *         parent : parent array.
 *         rank   : rank array (upper bound on subtree height).
 *         a      : first element.
 *         b      : second element.
 *
 * Returns:
 *         None
 *****************************************************************************/
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

int main(void)
{
    int parent[ELEMENT_COUNT];
    int rank[ELEMENT_COUNT];
    int i;

    for (i = 0; i < ELEMENT_COUNT; i++)
    {
        parent[i] = i;
        rank[i] = 0;
    }

    unionSets(parent, rank, 0, 1);
    unionSets(parent, rank, 1, 2);
    unionSets(parent, rank, 3, 4);

    printf("find(0) == find(2): %d\n", find(parent, 0) == find(parent, 2));
    printf("find(0) == find(3): %d\n", find(parent, 0) == find(parent, 3));

    return 0;
}
