// Kahn's algorithm in waves (parallel start groups), critical path length,
// cycle leaves services unstarted.

#include <stdio.h>

#define N 8

static const char *name[N] = {"mount", "udev", "network", "syslog", "database", "web", "ssh", "backup"};
static int need[N][N];                             /* need[a][b] = 1: b cannot start before a */

int main(void)
{
    need[0][1] = need[0][3] = 1;                   /* mount before udev and syslog */
    need[1][2] = 1;                                /* udev before network */
    need[2][5] = need[2][6] = 1;                   /* network before web and ssh */
    need[3][4] = 1;                                /* syslog before database */
    need[4][5] = need[4][7] = 1;                   /* database before web and backup */

    int indeg[N] = {0}, started = 0, wave = 0;
    for (int a = 0; a < N; a++)
        for (int b = 0; b < N; b++)
            indeg[b] += need[a][b];

    int done[N] = {0};
    while (started < N)
    {
        int ready[N], nready = 0;
        for (int i = 0; i < N; i++)
            if (!done[i] && indeg[i] == 0)
                ready[nready++] = i;               /* everything whose prerequisites are finished */
        if (nready == 0)
            break;                                 /* nothing can start: a cycle */
        printf("wave %d (in parallel):", ++wave);
        for (int k = 0; k < nready; k++)
        {
            printf(" %s", name[ready[k]]);
            done[ready[k]] = 1;
            started++;
        }
        printf("\n");
        for (int k = 0; k < nready; k++)
            for (int b = 0; b < N; b++)
                if (need[ready[k]][b])
                    indeg[b]--;
    }
    printf("%d waves, critical path length %d\n", wave, wave);

    need[5][0] = 1;                                /* web before mount: closes a loop */
    for (int i = 0; i < N; i++) indeg[i] = 0, done[i] = 0;
    for (int a = 0; a < N; a++)
        for (int b = 0; b < N; b++)
            indeg[b] += need[a][b];
    started = 0;
    for (int again = 1; again;)
    {
        again = 0;
        for (int i = 0; i < N; i++)
            if (!done[i] && indeg[i] == 0)
            {
                done[i] = 1; started++; again = 1;
                for (int b = 0; b < N; b++)
                    if (need[i][b]) indeg[b]--;
            }
    }
    printf("after adding 'web before mount': %d of %d services can start, the others are in or behind a cycle\n", started, N);
    return 0;
}
