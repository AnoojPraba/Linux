#include <stdio.h>

#define NUM_ITEMS 3
#define CAPACITY 50

/*****************************************************************************
 * Name: sortByRatioDescending
 *
 * Description:
 *         Simple insertion sort of items by value/weight ratio, descending.
 *
 * Inputs:
 *         weights : array of item weights, sorted in lockstep.
 *         values  : array of item values, sorted in lockstep.
 *         n       : number of items.
 *****************************************************************************/
void sortByRatioDescending(int weights[], int values[], int n)
{
    int i;
    int j;

    for (i = 1; i < n; i++)
    {
        int wKey = weights[i];
        int vKey = values[i];

        j = i - 1;

        while ((j >= 0) && (values[j] * wKey < vKey * weights[j]))
        {
            weights[j + 1] = weights[j];
            values[j + 1] = values[j];
            j--;
        }

        weights[j + 1] = wKey;
        values[j + 1] = vKey;
    }
}

/*****************************************************************************
 * Name: fractionalKnapsack
 *
 * Description:
 *         Greedy fractional knapsack: sort items by value/weight ratio
 *         descending, then take as much of the best-ratio item as fits,
 *         moving to the next item once capacity is exhausted or the item is
 *         fully taken. Unlike 0/1 knapsack (`46_DynamicProgramming/
 *         02_knapsack.c`), fractional items make greedy-by-ratio provably
 *         optimal: any leftover capacity can always be filled with a
 *         fraction of the next best item, so there is never a reason to
 *         prefer a lower-ratio item over any amount of a higher-ratio one.
 *         Greedy-by-ratio does NOT work for 0/1 knapsack - classic
 *         counterexample: capacity 50, item A (weight 10, value 60, ratio
 *         6), item B (weight 40, value 90, ratio 2.25). Greedy takes all of
 *         A (60) then can't fit B, total 60; but taking only B gives 90 -
 *         greedy loses because 0/1 knapsack can't split B to fill the
 *         remaining capacity the way the fractional version can.
 *
 * Inputs:
 *         weights  : array of item weights, sorted by ratio descending.
 *         values   : array of item values, sorted by ratio descending.
 *         n        : number of items.
 *         capacity : total knapsack capacity.
 *
 * Returns:
 *         The maximum total value achievable (as an int, truncating any
 *         fractional value contribution).
 *****************************************************************************/
int fractionalKnapsack(int weights[], int values[], int n, int capacity)
{
    int totalValue;
    int remaining;
    int i;

    totalValue = 0;
    remaining = capacity;

    for (i = 0; (i < n) && (remaining > 0); i++)
    {
        if (weights[i] <= remaining)
        {
            totalValue += values[i];
            remaining -= weights[i];
        }
        else
        {
            totalValue += values[i] * remaining / weights[i];
            remaining = 0;
        }
    }

    return totalValue;
}

int main()
{
    int weights[NUM_ITEMS] = {10, 20, 30};
    int values[NUM_ITEMS] = {60, 100, 120};

    sortByRatioDescending(weights, values, NUM_ITEMS);

    printf("max fractional knapsack value for capacity %d = %d\n", CAPACITY,
           fractionalKnapsack(weights, values, NUM_ITEMS, CAPACITY));

    return 0;
}
