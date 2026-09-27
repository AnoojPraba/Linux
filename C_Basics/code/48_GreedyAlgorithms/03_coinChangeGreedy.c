#include <stdio.h>

#define NUM_CANONICAL_COINS 4
#define CANONICAL_TARGET 63

#define NUM_NONCANONICAL_COINS 3
#define NONCANONICAL_TARGET 6

/*****************************************************************************
 * Name: greedyCoinCount
 *
 * Description:
 *         Greedy coin change: always take as many of the largest coin
 *         denomination as fit, then move to the next-largest, and so on.
 *         coins must be sorted descending. For a "canonical" coin system
 *         (like US coins 1/5/10/25) this happens to give the true minimum
 *         coin count, but greedy is not correct in general - see main() for
 *         a non-canonical counterexample where it fails. When the coin
 *         system isn't known to be canonical, use the DP solution in
 *         `46_DynamicProgramming/05_coinChangeMinCoins.c` instead, which is
 *         always correct regardless of the coin system.
 *
 * Inputs:
 *         coins  : coin denominations, sorted descending.
 *         n      : number of denominations.
 *         amount : target amount to make change for.
 *
 * Returns:
 *         The number of coins greedy uses (not necessarily optimal).
 *****************************************************************************/
int greedyCoinCount(int coins[], int n, int amount)
{
    int count;
    int i;

    count = 0;

    for (i = 0; i < n; i++)
    {
        count += amount / coins[i];
        amount %= coins[i];
    }

    return count;
}

int main()
{
    int canonicalCoins[NUM_CANONICAL_COINS] = {25, 10, 5, 1};
    int nonCanonicalCoins[NUM_NONCANONICAL_COINS] = {4, 3, 1};

    printf("canonical US coins, amount %d: greedy uses %d coins (optimal)\n",
           CANONICAL_TARGET, greedyCoinCount(canonicalCoins, NUM_CANONICAL_COINS,
                                              CANONICAL_TARGET));

    // Non-canonical coin system {1, 3, 4}, target 6: greedy takes 4, then
    // 1, then 1 -> 3 coins (4+1+1). The optimal answer is 2 coins (3+3).
    // Greedy's "always take the largest that fits" locally-optimal choice
    // does not lead to the globally optimal count here - this is the
    // classic example that greedy coin change is only correct for coin
    // systems proven canonical, not in general.
    printf("non-canonical coins {4,3,1}, amount %d: greedy uses %d coins "
           "(optimal is 2, via 3+3)\n",
           NONCANONICAL_TARGET, greedyCoinCount(nonCanonicalCoins,
                                                 NUM_NONCANONICAL_COINS,
                                                 NONCANONICAL_TARGET));

    return 0;
}
