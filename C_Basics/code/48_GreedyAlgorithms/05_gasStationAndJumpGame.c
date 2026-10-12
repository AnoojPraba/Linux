// Two classic greedy scans: gas station circuit (where to start) and jump game
// (can you reach the end, minimum jumps).

#include <stdio.h>

static int gas_start(const int *gas, const int *cost, int n)
{
    int total = 0, tank = 0, start = 0;
    for (int i = 0; i < n; i++)
    {
        total += gas[i] - cost[i];
        tank += gas[i] - cost[i];
        if (tank < 0)                    /* cannot get past i from this start, nor from any start before i */
        {
            start = i + 1;
            tank = 0;
        }
    }
    return total >= 0 ? start : -1;      /* enough gas overall: the candidate start works */
}

static int can_reach(const int *a, int n)
{
    int far = 0;
    for (int i = 0; i < n; i++)
    {
        if (i > far) return 0;           /* stuck before reaching i */
        if (i + a[i] > far) far = i + a[i];
    }
    return 1;
}

static int min_jumps(const int *a, int n)
{
    int jumps = 0, end = 0, far = 0;
    for (int i = 0; i < n - 1; i++)
    {
        if (i + a[i] > far) far = i + a[i];
        if (i == end)                    /* must jump now: take the best jump seen in this window */
        {
            jumps++;
            end = far;
            if (end >= n - 1) break;
        }
    }
    return jumps;
}

int main(void)
{
    int gas[] = {1, 2, 3, 4, 5}, cost[] = {3, 4, 5, 1, 2};
    printf("gas station start index = %d\n", gas_start(gas, cost, 5));
    int bad_gas[] = {2, 3, 4}, bad_cost[] = {3, 4, 3};
    printf("impossible circuit start = %d\n", gas_start(bad_gas, bad_cost, 3));

    int a[] = {2, 3, 1, 1, 4}, b[] = {3, 2, 1, 0, 4};
    printf("jump game {2,3,1,1,4}: reachable %d, minimum jumps %d\n", can_reach(a, 5), min_jumps(a, 5));
    printf("jump game {3,2,1,0,4}: reachable %d\n", can_reach(b, 5));
    return 0;
}
