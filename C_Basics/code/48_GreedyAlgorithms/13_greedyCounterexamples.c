// When greedy fails: coin change with 1/3/4, 0/1 knapsack by ratio and the DP
// answers that fix them.

#include <stdio.h>
#include <stdlib.h>

static int greedy_coins(const int *c, int n, int amt)     /* coins in descending order */
{
    int count = 0;
    for (int i = 0; i < n; i++)
        for (; amt >= c[i]; amt -= c[i]) count++;
    return amt ? -1 : count;
}

static int dp_coins(const int *c, int n, int amt)
{
    int best[64];
    best[0] = 0;
    for (int x = 1; x <= amt; x++)
    {
        best[x] = 1 << 20;
        for (int i = 0; i < n; i++)
            if (c[i] <= x && best[x - c[i]] + 1 < best[x]) best[x] = best[x - c[i]] + 1;
    }
    return best[amt];
}

typedef struct { int w, v; } Item;
static int by_ratio(const void *a, const void *b)
{
    const Item *x = a, *y = b;
    return y->v * x->w - x->v * y->w;                      /* higher value per weight first */
}

int main(void)
{
    int coins[] = {4, 3, 1};
    printf("amount 6 with coins 4/3/1: greedy %d coins, optimal %d coins\n", greedy_coins(coins, 3, 6), dp_coins(coins, 3, 6));
    int us[] = {25, 10, 5, 1};
    printf("amount 63 with 25/10/5/1  : greedy %d coins, optimal %d coins (greedy is fine here)\n", greedy_coins(us, 4, 63), dp_coins(us, 4, 63));

    Item items[] = {{10, 60}, {20, 100}, {30, 120}};      /* capacity 50 */
    qsort(items, 3, sizeof items[0], by_ratio);
    int cap = 50, greedy_value = 0;
    for (int i = 0; i < 3; i++)
        if (items[i].w <= cap) { cap -= items[i].w; greedy_value += items[i].v; }
    int dp[51] = {0};
    for (int i = 0; i < 3; i++)
        for (int c = 50; c >= items[i].w; c--)
            if (dp[c - items[i].w] + items[i].v > dp[c]) dp[c] = dp[c - items[i].w] + items[i].v;
    printf("0/1 knapsack, capacity 50: greedy by ratio %d, optimal %d\n", greedy_value, dp[50]);
    return 0;
}
