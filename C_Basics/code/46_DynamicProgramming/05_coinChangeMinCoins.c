#include <stdio.h>

#define NUM_COINS 3
#define TARGET_AMOUNT 11
#define INFEASIBLE (TARGET_AMOUNT + 1)

/*****************************************************************************
 * Name: minCoinsUnbounded
 *
 * Description:
 *         Minimum number of coins needed to make amount, given unlimited
 *         supply of each coin denomination. This is the "unbounded
 *         knapsack" DP variant: unlike 02_knapsack.c's 0/1 knapsack, where
 *         each item can be used at most once (the recurrence pulls from row
 *         i-1, i.e. "earlier items only"), here each coin can be reused any
 *         number of times, so the recurrence pulls from the *same* row
 *         (best[amount - coins[i]] may itself already include coin i).
 *         best[a] = the fewest coins that sum to exactly a, or INFEASIBLE if
 *         no combination of coins sums to a.
 *
 * Inputs:
 *         coins  : array of coin denominations.
 *         n      : number of coin denominations.
 *         amount : the target amount to make change for.
 *
 * Returns:
 *         The minimum number of coins to make amount, or INFEASIBLE if it
 *         cannot be made.
 *****************************************************************************/
int minCoinsUnbounded(int coins[], int n, int amount)
{
    int best[TARGET_AMOUNT + 1];
    int a;
    int i;

    best[0] = 0;

    for (a = 1; a <= amount; a++)
    {
        best[a] = INFEASIBLE;

        for (i = 0; i < n; i++)
        {
            if ((coins[i] <= a) && (best[a - coins[i]] + 1 < best[a]))
            {
                best[a] = best[a - coins[i]] + 1;
            }
        }
    }

    return best[amount];
}

int main()
{
    int coins[NUM_COINS] = {1, 5, 6};
    int result;

    result = minCoinsUnbounded(coins, NUM_COINS, TARGET_AMOUNT);

    if (result >= INFEASIBLE)
    {
        printf("no way to make amount %d\n", TARGET_AMOUNT);
    }
    else
    {
        printf("min coins for amount %d = %d\n", TARGET_AMOUNT, result);
    }

    return 0;
}
