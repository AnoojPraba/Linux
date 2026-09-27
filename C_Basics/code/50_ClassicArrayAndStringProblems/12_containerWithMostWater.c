#include <stdio.h>

#define ARR_SIZE 9

/*****************************************************************************
 * Name: maxArea
 *
 * Description:
 *         Finds two lines that, together with the x-axis, form the
 *         container holding the most water. Uses two pointers starting at
 *         both ends: the area for the current pair is limited by the
 *         SHORTER of the two lines times the distance between them, so the
 *         pointer at the shorter line is always the one moved inward -
 *         moving the taller line's pointer instead can only shrink the
 *         width while the height stays capped by the same shorter line (or
 *         gets worse), so it can never improve the area. Moving the
 *         shorter line's pointer is the only move that has a chance of
 *         finding a taller line to increase the area, even though the
 *         width decreases either way.
 *
 * Inputs:
 *         heights : array of line heights.
 *         n       : number of elements in heights.
 *
 * Returns:
 *         Maximum container area found.
 *****************************************************************************/
int maxArea(int heights[], int n)
{
    int left;
    int right;
    int best;
    int width;
    int height;
    int area;

    left = 0;
    right = n - 1;
    best = 0;

    while (left < right)
    {
        width = right - left;
        height = (heights[left] < heights[right]) ? heights[left] : heights[right];
        area = width * height;
        if (area > best)
        {
            best = area;
        }

        if (heights[left] < heights[right])
        {
            left++;
        }
        else
        {
            right--;
        }
    }

    return best;
}

int main(void)
{
    int heights[ARR_SIZE] = {1, 8, 6, 2, 5, 4, 8, 3, 7};

    printf("max area = %d\n", maxArea(heights, ARR_SIZE));

    return 0;
}
