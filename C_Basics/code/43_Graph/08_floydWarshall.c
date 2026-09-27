#include <stdio.h>

#define NUM_VERTICES 4
#define INF (1 << 20)

// Floyd-Warshall: DP over which intermediate vertices are allowed on a
// path. dist[i][j] after considering vertex k is the shortest of the
// current dist[i][j] and routing through k (dist[i][k] + dist[k][j]).
// INF is kept far below INT_MAX so two INF values can be added without
// overflowing.
void floydWarshall(int dist[NUM_VERTICES][NUM_VERTICES])
{
    int k;
    int i;
    int j;

    for (k = 0; k < NUM_VERTICES; k++)
    {
        for (i = 0; i < NUM_VERTICES; i++)
        {
            for (j = 0; j < NUM_VERTICES; j++)
            {
                if ((dist[i][k] + dist[k][j]) < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

void printMatrix(int dist[NUM_VERTICES][NUM_VERTICES])
{
    int i;
    int j;

    for (i = 0; i < NUM_VERTICES; i++)
    {
        for (j = 0; j < NUM_VERTICES; j++)
        {
            if (dist[i][j] >= INF)
            {
                printf("  INF");
            }
            else
            {
                printf("%5d", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int main(void)
{
    int dist[NUM_VERTICES][NUM_VERTICES] = {
        {0, 3, INF, 7},
        {8, 0, 2, INF},
        {5, INF, 0, 1},
        {2, INF, INF, 0}
    };

    printf("all-pairs shortest distances:\n");
    floydWarshall(dist);
    printMatrix(dist);

    return 0;
}
