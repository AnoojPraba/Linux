#include <stdio.h>

#define ARR_SIZE 12

/*****************************************************************************
 * Name: trapRainWater
 *
 * Description:
 *         Computes how much water can be trapped between elevation bars
 *         after raining, using two pointers starting at each end while
 *         tracking the running max height seen from the left and from the
 *         right. At each step, move whichever pointer has the smaller max,
 *         since the water level at that position is bounded by the smaller
 *         of the two maxes - the taller side can never be the limiting
 *         factor, so it's safe to resolve the smaller side's contribution
 *         immediately and move inward.
 *
 * Inputs:
 *         heights : array of elevation heights.
 *         n       : number of elements in heights.
 *
 * Returns:
 *         Total units of water trapped.
 *****************************************************************************/
int trapRainWater(int heights[], int n)
{
    int left;
    int right;
    int maxLeft;
    int maxRight;
    int total;

    left = 0;
    right = n - 1;
    maxLeft = 0;
    maxRight = 0;
    total = 0;

    while (left < right)
    {
        if (heights[left] < heights[right])
        {
            if (heights[left] > maxLeft)
            {
                maxLeft = heights[left];
            }
            else
            {
                total += maxLeft - heights[left];
            }
            left++;
        }
        else
        {
            if (heights[right] > maxRight)
            {
                maxRight = heights[right];
            }
            else
            {
                total += maxRight - heights[right];
            }
            right--;
        }
    }

    return total;
}

int main(void)
{
    int heights[ARR_SIZE] = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};

    printf("water trapped = %d\n", trapRainWater(heights, ARR_SIZE));

    return 0;
}
