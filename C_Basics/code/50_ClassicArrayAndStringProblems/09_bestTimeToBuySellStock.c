#include <stdio.h>
#include <limits.h>

#define ARR_SIZE 6

/*****************************************************************************
 * Name: maxProfit
 *
 * Description:
 *         Finds the maximum profit from a single buy-then-sell transaction
 *         given daily prices, in a single O(n) pass: track the minimum
 *         price seen so far while scanning forward, and at each day
 *         compute the profit from selling today at that day's price minus
 *         the running minimum, keeping the best such profit seen.
 *
 * Inputs:
 *         prices : array of daily prices.
 *         n      : number of days.
 *
 * Returns:
 *         Maximum achievable profit, or 0 if no profitable transaction
 *         exists.
 *****************************************************************************/
int maxProfit(int prices[], int n)
{
    int minPriceSoFar;
    int bestProfit;
    int i;
    int profitToday;

    if (n == 0)
    {
        return 0;
    }

    minPriceSoFar = INT_MAX;
    bestProfit = 0;

    for (i = 0; i < n; i++)
    {
        if (prices[i] < minPriceSoFar)
        {
            minPriceSoFar = prices[i];
        }
        else
        {
            profitToday = prices[i] - minPriceSoFar;
            if (profitToday > bestProfit)
            {
                bestProfit = profitToday;
            }
        }
    }

    return bestProfit;
}

int main(void)
{
    int prices[ARR_SIZE] = {7, 1, 5, 3, 6, 4};

    printf("max profit = %d\n", maxProfit(prices, ARR_SIZE));

    return 0;
}
