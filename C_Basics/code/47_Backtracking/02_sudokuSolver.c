#include <stdio.h>

#define GRID_SIZE 9
#define BOX_SIZE 3
#define EMPTY_CELL 0

/*****************************************************************************
 * Name: isValidPlacement
 *
 * Description:
 *         Checks whether placing digit at (row, col) violates the row,
 *         column, or 3x3 box constraint.
 *
 * Inputs:
 *         grid  : the 9x9 sudoku grid.
 *         row   : row of the candidate placement.
 *         col   : column of the candidate placement.
 *         digit : the digit being tried.
 *
 * Returns:
 *         1 if the placement is valid, 0 otherwise.
 *****************************************************************************/
int isValidPlacement(int grid[GRID_SIZE][GRID_SIZE], int row, int col, int digit)
{
    int i;
    int j;
    int boxRow;
    int boxCol;

    for (i = 0; i < GRID_SIZE; i++)
    {
        if ((grid[row][i] == digit) || (grid[i][col] == digit))
        {
            return 0;
        }
    }

    boxRow = (row / BOX_SIZE) * BOX_SIZE;
    boxCol = (col / BOX_SIZE) * BOX_SIZE;

    for (i = boxRow; i < boxRow + BOX_SIZE; i++)
    {
        for (j = boxCol; j < boxCol + BOX_SIZE; j++)
        {
            if (grid[i][j] == digit)
            {
                return 0;
            }
        }
    }

    return 1;
}

/*****************************************************************************
 * Name: solveSudoku
 *
 * Description:
 *         Backtracking sudoku solver: scans for the first empty cell, tries
 *         digits 1-9 in it (choose), recurses on the rest of the grid, and
 *         resets the cell back to EMPTY_CELL (un-choose) if no digit leads
 *         to a solution. Returns immediately once no empty cell remains.
 *
 * Inputs:
 *         grid : the 9x9 sudoku grid, modified in place.
 *
 * Returns:
 *         1 if the grid was solved, 0 if it is unsolvable as given.
 *****************************************************************************/
int solveSudoku(int grid[GRID_SIZE][GRID_SIZE])
{
    int row;
    int col;
    int digit;
    int emptyRow = -1;
    int emptyCol = -1;

    for (row = 0; row < GRID_SIZE; row++)
    {
        for (col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == EMPTY_CELL)
            {
                emptyRow = row;
                emptyCol = col;
            }
        }
    }

    if (emptyRow == -1)
    {
        return 1;
    }

    for (digit = 1; digit <= GRID_SIZE; digit++)
    {
        if (isValidPlacement(grid, emptyRow, emptyCol, digit))
        {
            grid[emptyRow][emptyCol] = digit;

            if (solveSudoku(grid))
            {
                return 1;
            }

            grid[emptyRow][emptyCol] = EMPTY_CELL;
        }
    }

    return 0;
}

/*****************************************************************************
 * Name: printGrid
 *
 * Description:
 *         Prints a 9x9 sudoku grid.
 *
 * Inputs:
 *         grid : the 9x9 sudoku grid.
 *****************************************************************************/
void printGrid(int grid[GRID_SIZE][GRID_SIZE])
{
    int row;
    int col;

    for (row = 0; row < GRID_SIZE; row++)
    {
        for (col = 0; col < GRID_SIZE; col++)
        {
            printf("%d ", grid[row][col]);
        }

        printf("\n");
    }
}

int main()
{
    int grid[GRID_SIZE][GRID_SIZE] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    if (solveSudoku(grid))
    {
        printf("solved sudoku:\n");
        printGrid(grid);
    }
    else
    {
        printf("no solution exists\n");
    }

    return 0;
}
