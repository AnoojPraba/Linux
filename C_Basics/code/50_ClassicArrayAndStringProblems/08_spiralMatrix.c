#include <stdio.h>

#define ROWS 3
#define COLS 4

/*****************************************************************************
 * Name: printSpiral
 *
 * Description:
 *         Traverses a 2D matrix in spiral order by maintaining four
 *         boundaries - top, bottom, left, right - and walking each side in
 *         turn (left-to-right across the top row, top-to-bottom down the
 *         right column, right-to-left across the bottom row, bottom-to-top
 *         up the left column), shrinking the corresponding boundary after
 *         each side so the next lap spirals inward.
 *
 * Inputs:
 *         matrix : 2D array to traverse.
 *         rows   : number of rows in matrix.
 *         cols   : number of columns in matrix.
 *
 * Returns:
 *         None
 *****************************************************************************/
void printSpiral(int matrix[ROWS][COLS], int rows, int cols)
{
    int top;
    int bottom;
    int left;
    int right;
    int i;

    top = 0;
    bottom = rows - 1;
    left = 0;
    right = cols - 1;

    while ((top <= bottom) && (left <= right))
    {
        for (i = left; i <= right; i++)
        {
            printf("%d ", matrix[top][i]);
        }
        top++;

        for (i = top; i <= bottom; i++)
        {
            printf("%d ", matrix[i][right]);
        }
        right--;

        if (top <= bottom)
        {
            for (i = right; i >= left; i--)
            {
                printf("%d ", matrix[bottom][i]);
            }
            bottom--;
        }

        if (left <= right)
        {
            for (i = bottom; i >= top; i--)
            {
                printf("%d ", matrix[i][left]);
            }
            left++;
        }
    }
}

int main(void)
{
    int matrix[ROWS][COLS] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};

    printf("spiral order: ");
    printSpiral(matrix, ROWS, COLS);
    printf("\n");

    return 0;
}
