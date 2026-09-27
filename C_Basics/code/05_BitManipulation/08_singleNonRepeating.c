#include <stdio.h>

#define ARRAY_LEN 7

/*****************************************************************************
 * Name: findSingleNonRepeating
 *
 * Description:
 *         Every element in arr appears exactly twice except one, which
 *         appears once. XORing all elements together cancels out every
 *         pair (x ^ x == 0), and XORing with 0 leaves a value unchanged
 *         (x ^ 0 == x), so what remains after XORing the whole array is
 *         exactly the single non-repeating element. Runs in O(n) time
 *         using O(1) extra space.
 *
 * Inputs:
 *         arr : the array to search.
 *         len : number of elements in arr.
 *
 * Returns:
 *         The single element that does not repeat.
 *****************************************************************************/
int findSingleNonRepeating(const int arr[], int len)
{
    int result = 0;
    int i;

    for (i = 0; i < len; i++)
    {
        result = result ^ arr[i];
    }

    return result;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates findSingleNonRepeating on a sample array.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main(void)
{
    int arr[ARRAY_LEN] = { 4, 1, 2, 1, 2, 4, 9 };

    printf("Single non-repeating element: %d\n", findSingleNonRepeating(arr, ARRAY_LEN));

    return 0;
}
