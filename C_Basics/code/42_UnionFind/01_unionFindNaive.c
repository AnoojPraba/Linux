#include <stdio.h>

#define ELEMENT_COUNT 10

/*****************************************************************************
 * Name: find
 *
 * Description:
 *         Follows parent pointers to the root of the set containing an
 *         element, with no path compression.
 *
 * Inputs:
 *         parent  : parent array.
 *         element : element to find the root of.
 *
 * Returns:
 *         Root of the set containing element.
 *****************************************************************************/
int find(const int *parent, int element)
{
    while (parent[element] != element)
    {
        element = parent[element];
    }
    return element;
}

/*****************************************************************************
 * Name: unionSets
 *
 * Description:
 *         Merges the sets containing two elements by making one root point
 *         to the other, with no rank/size heuristic.
 *
 * Inputs:
 *         parent : parent array.
 *         a      : first element.
 *         b      : second element.
 *
 * Returns:
 *         None
 *****************************************************************************/
void unionSets(int *parent, int a, int b)
{
    int rootA = find(parent, a);
    int rootB = find(parent, b);

    if (rootA != rootB)
    {
        parent[rootA] = rootB;
    }
}

int main(void)
{
    int parent[ELEMENT_COUNT];
    int i;

    for (i = 0; i < ELEMENT_COUNT; i++)
    {
        parent[i] = i;
    }

    unionSets(parent, 0, 1);
    unionSets(parent, 1, 2);
    unionSets(parent, 3, 4);

    printf("find(0) == find(2): %d\n", find(parent, 0) == find(parent, 2));
    printf("find(0) == find(3): %d\n", find(parent, 0) == find(parent, 3));

    return 0;
}
