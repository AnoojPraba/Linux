#include <stdio.h>

#define ARR_SIZE 4

/*****************************************************************************
 * Name: swap
 *
 * Description:
 *         Swaps two integers in place.
 *
 * Inputs:
 *         a : pointer to the first integer.
 *         b : pointer to the second integer.
 *****************************************************************************/
void swap(int *a, int *b)
{
    int tmp = *a;

    *a = *b;
    *b = tmp;
}

/*****************************************************************************
 * Name: printArr
 *
 * Description:
 *         Prints an array of integers on one line.
 *
 * Inputs:
 *         arr : the array to print.
 *         n   : number of elements in arr.
 *****************************************************************************/
void printArr(int arr[], int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");
}

/*****************************************************************************
 * Name: permute
 *
 * Description:
 *         Swap-based backtracking permutation generator. At each recursion
 *         level, every remaining element is tried in the current slot
 *         (choose = swap it into place), the rest of the array is permuted
 *         recursively, and then the swap is undone (un-choose) so the next
 *         candidate can be tried in that slot.
 *
 * Inputs:
 *         arr   : the array being permuted, modified in place.
 *         start : index of the slot currently being filled.
 *         n     : number of elements in arr.
 *****************************************************************************/
void permute(int arr[], int start, int n)
{
    int i;

    if (start == n)
    {
        printArr(arr, n);
        return;
    }

    for (i = start; i < n; i++)
    {
        swap(&arr[start], &arr[i]);
        permute(arr, start + 1, n);
        swap(&arr[start], &arr[i]);
    }
}

int main()
{
    int arr[ARR_SIZE] = {1, 2, 3, 4};

    permute(arr, 0, ARR_SIZE);

    return 0;
}
