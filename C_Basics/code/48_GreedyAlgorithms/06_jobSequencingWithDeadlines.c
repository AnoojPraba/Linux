// Job sequencing with deadlines: maximize profit by scheduling the most
// profitable jobs in the latest free slot, plus the union-find speedup.

#include <stdio.h>
#include <stdlib.h>

typedef struct { char id; int deadline, profit; } Job;

static int by_profit_desc(const void *a, const void *b)
{
    return ((const Job *)b)->profit - ((const Job *)a)->profit;
}

int main(void)
{
    Job jobs[] = {{'a', 2, 100}, {'b', 1, 19}, {'c', 2, 27}, {'d', 1, 25}, {'e', 3, 15}};
    int n = sizeof jobs / sizeof jobs[0];
    qsort(jobs, n, sizeof jobs[0], by_profit_desc);    /* most profitable first */

    char slot[8] = {0};                                /* slot[t] = job run during time unit t (1-based) */
    int total = 0;
    for (int i = 0; i < n; i++)
        for (int t = jobs[i].deadline; t >= 1; t--)    /* latest free slot at or before the deadline */
            if (!slot[t])
            {
                slot[t] = jobs[i].id;
                total += jobs[i].profit;
                break;
            }
    printf("schedule:");
    for (int t = 1; t <= 3; t++) printf(" t%d=%c", t, slot[t] ? slot[t] : '-');
    printf("\ntotal profit = %d\n", total);
    return 0;
}
