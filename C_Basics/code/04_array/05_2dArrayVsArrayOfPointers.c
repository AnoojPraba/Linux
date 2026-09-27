#include <stdio.h>
#include <stdlib.h>

#define ROWS 3
#define COLS 4

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates the memory layout difference between a true 2D
 *         array (single contiguous block, row-major, `matrix[i][j]`
 *         computed as `matrix + i*COLS + j` by the compiler) and an
 *         array of pointers (each row a separate malloc, scattered on the
 *         heap, `matrix[i][j]` requiring an actual pointer dereference of
 *         `matrix[i]` before indexing). Prints consecutive row addresses
 *         for each so the contiguous-vs-scattered difference is visible.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    int trueMatrix[ROWS][COLS];
    int *pointerMatrix[ROWS];
    int i;
    int j;

    // True 2D array: one contiguous block of ROWS * COLS * sizeof(int) bytes.
    // Consecutive row addresses differ by exactly COLS * sizeof(int), since
    // the compiler computes trueMatrix[i][j] via trueMatrix + i*COLS + j.
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            trueMatrix[i][j] = (i * COLS) + j;
        }
    }

    printf("True 2D array row addresses (contiguous, fixed stride):\n");
    for (i = 0; i < ROWS; i++)
    {
        printf("  row %d: %p\n", i, (void *)trueMatrix[i]);
    }

    // Array of pointers: each row is a separately malloc'd block, so rows
    // are NOT guaranteed contiguous and can be scattered anywhere on the
    // heap. matrix[i][j] involves a real pointer dereference of matrix[i]
    // (a load from pointerMatrix), then an offset into that block, unlike
    // the pure arithmetic used for the true 2D array above.
    for (i = 0; i < ROWS; i++)
    {
        pointerMatrix[i] = malloc(COLS * sizeof(int));
        for (j = 0; j < COLS; j++)
        {
            pointerMatrix[i][j] = (i * COLS) + j;
        }
    }

    printf("Array-of-pointers row addresses (scattered, unpredictable):\n");
    for (i = 0; i < ROWS; i++)
    {
        printf("  row %d: %p\n", i, (void *)pointerMatrix[i]);
    }

    // Practical implications:
    // - True 2D array: one contiguous block gives better cache locality
    //   (spatial locality across rows) and needs only one allocation/
    //   deallocation, but ROWS/COLS must be known at compile time (or via
    //   a C99+ VLA, which has its own stack-size and lifetime tradeoffs -
    //   see ../22_AdvancedArrays/01_variableLengthArray.c).
    // - Array of pointers: allows genuinely ragged/jagged rows (different
    //   lengths per row) and dimensions decided at runtime via simple
    //   per-row malloc/free, at the cost of scattered memory and an extra
    //   pointer dereference per access.
    for (i = 0; i < ROWS; i++)
    {
        free(pointerMatrix[i]);
    }

    return 0;
}
