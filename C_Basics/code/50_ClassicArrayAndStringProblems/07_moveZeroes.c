#include <stdio.h>

#define ARR_SIZE 8

/*****************************************************************************
 * Name: moveZeroes
 *
 * Description:
 *         Moves all zeroes in an array to the end while preserving the
 *         relative order of non-zero elements, in-place and in a single
 *         O(n) pass. Uses a "write" pointer tracking where the next
 *         non-zero element belongs: every non-zero element seen while
 *         scanning is copied to the write position and the write pointer
 *         advances; once the scan finishes, every remaining slot from the
 *         write pointer onward is zeroed out.
 *
 * Inputs:
 *         arr : array to modify in place.
 *         n   : number of elements in arr.
 *
 * Returns:
 *         None
 *****************************************************************************/
void moveZeroes(int arr[], int n)
{
    int writePos;
    int i;

    writePos = 0;
    for (i = 0; i < n; i++)
    {
        if (arr[i] != 0)
        {
            arr[writePos] = arr[i];
            writePos++;
        }
    }

    for (; writePos < n; writePos++)
    {
        arr[writePos] = 0;
    }
}

int main(void)
{
    int arr[ARR_SIZE] = {0, 1, 0, 3, 12, 0, 5, 0};
    int i;

    moveZeroes(arr, ARR_SIZE);

    printf("result: ");
    for (i = 0; i < ARR_SIZE; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
