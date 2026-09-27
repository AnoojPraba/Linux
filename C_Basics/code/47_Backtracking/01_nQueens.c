#include <stdio.h>

#define BOARD_SIZE 8

int gSolutionCount = 0;
int gCols[BOARD_SIZE];
int gStopAtFirst = 0;
int gFound = 0;

/*****************************************************************************
 * Name: printFirstSolution
 *
 * Description:
 *         Prints one N-Queens board layout, one queen column index per row.
 *
 * Inputs:
 *         n : the board size.
 *****************************************************************************/
void printFirstSolution(int n)
{
    int row;
    int col;

    for (row = 0; row < n; row++)
    {
        for (col = 0; col < n; col++)
        {
            printf("%c ", (gCols[row] == col) ? 'Q' : '.');
        }

        printf("\n");
    }
}

/*****************************************************************************
 * Name: isSafe
 *
 * Description:
 *         Checks whether placing a queen at (row, col) conflicts with any
 *         queen already placed in an earlier row (same column, or either
 *         diagonal).
 *
 * Inputs:
 *         row : row of the candidate placement.
 *         col : column of the candidate placement.
 *
 * Returns:
 *         1 if the placement is safe, 0 otherwise.
 *****************************************************************************/
int isSafe(int row, int col)
{
    int prevRow;

    for (prevRow = 0; prevRow < row; prevRow++)
    {
        int prevCol = gCols[prevRow];
        int rowDiff = row - prevRow;
        int colDiff = (prevCol > col) ? (prevCol - col) : (col - prevCol);

        if ((prevCol == col) || (rowDiff == colDiff))
        {
            return 0;
        }
    }

    return 1;
}

/*****************************************************************************
 * Name: solveNQueens
 *
 * Description:
 *         Backtracking N-Queens: for each row, try every column - choose a
 *         column, recurse into the next row, and if no completion works,
 *         un-choose (the column simply gets overwritten on the next try, so
 *         there is nothing else to undo). Counts every full solution and
 *         remembers the first one found for display.
 *
 * Inputs:
 *         row : the row currently being filled.
 *         n   : the board size.
 *****************************************************************************/
void solveNQueens(int row, int n)
{
    int col;

    if (row == n)
    {
        gSolutionCount++;
        gFound = 1;
        return;
    }

    for (col = 0; col < n; col++)
    {
        if (gStopAtFirst && gFound)
        {
            return;
        }

        if (isSafe(row, col))
        {
            gCols[row] = col;
            solveNQueens(row + 1, n);
        }
    }
}

int main()
{
    solveNQueens(0, BOARD_SIZE);
    printf("N-Queens (N=%d): %d solutions found\n", BOARD_SIZE, gSolutionCount);

    gSolutionCount = 0;
    gFound = 0;
    gStopAtFirst = 1;
    solveNQueens(0, BOARD_SIZE);

    printf("One example solution:\n");
    printFirstSolution(BOARD_SIZE);

    return 0;
}
