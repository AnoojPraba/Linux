#include <stdio.h>

#define ARR_SIZE 7

void reverseRange(int arr[], int left, int right)
{
    int temp;

    while (left < right)
    {
        temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}

/*****************************************************************************
 * Name: rotateRight
 *
 * Description:
 *         Rotates an array right by k positions in-place, O(1) extra space,
 *         using the "reverse three times" trick: reversing the whole array
 *         moves the last k elements to the front but in reversed order (and
 *         likewise for the remaining n-k elements) - reversing each of
 *         those two segments individually then un-reverses them back into
 *         correct relative order, leaving the whole array correctly rotated.
 *
 * Inputs:
 *         arr : array to rotate in place.
 *         n   : number of elements in arr.
 *         k   : number of positions to rotate right by (may exceed n).
 *
 * Returns:
 *         None
 *****************************************************************************/
void rotateRight(int arr[], int n, int k)
{
    k = k % n;
    if (k == 0)
    {
        return;
    }

    reverseRange(arr, 0, n - 1);
    reverseRange(arr, 0, k - 1);
    reverseRange(arr, k, n - 1);
}

int main(void)
{
    int arr[ARR_SIZE] = {1, 2, 3, 4, 5, 6, 7};
    int i;

    rotateRight(arr, ARR_SIZE, 3);

    printf("rotated: ");
    for (i = 0; i < ARR_SIZE; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
