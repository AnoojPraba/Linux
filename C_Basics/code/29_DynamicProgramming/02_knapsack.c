#include <stdio.h>

#define NUM_ITEMS 4
#define CAPACITY 7

int max(int a, int b)
{
    return (a > b) ? a : b;
}

// 0/1 knapsack: table[i][w] = the best value achievable using only the
// first i items with a knapsack capacity of w. Each cell only needs the
// row above it, which is what makes this a 2D DP table instead of a single
// array like the 1D fibonacci table in 01_fibMemoVsTabulation.c.
int knapsack(int weights[], int values[], int n, int capacity)
{
    int table[NUM_ITEMS + 1][CAPACITY + 1];
    int i;
    int w;

    for (i = 0; i <= n; i++)
    {
        for (w = 0; w <= capacity; w++)
        {
            if ((i == 0) || (w == 0))
            {
                table[i][w] = 0;
            }
            else if (weights[i - 1] <= w)
            {
                // Either skip item i-1 (carry down the row above), or take
                // it (its value plus the best result for the remaining
                // capacity using only earlier items) - whichever is larger.
                int skipItem = table[i - 1][w];
                int takeItem = values[i - 1] + table[i - 1][w - weights[i - 1]];

                table[i][w] = max(skipItem, takeItem);
            }
            else
            {
                table[i][w] = table[i - 1][w];
            }
        }
    }

    return table[n][capacity];
}

int main()
{
    int weights[NUM_ITEMS] = {1, 3, 4, 5};
    int values[NUM_ITEMS] = {1, 4, 5, 7};

    printf("max value for capacity %d = %d\n", CAPACITY,
           knapsack(weights, values, NUM_ITEMS, CAPACITY));

    return 0;
}
