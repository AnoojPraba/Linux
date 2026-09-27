#include <stdio.h>

void printArray(int arr[], int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Repeatedly walk the array swapping adjacent out-of-order elements. Each
// pass bubbles the next-largest remaining element to its final position, so
// the inner loop's upper bound shrinks by one every pass.
void bubbleSort(int arr[], int size)
{
    int i;
    int j;

    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - 1 - i; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int tmp = arr[j];

                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int main()
{
    int arr[] = {8, 3, 5, 1, 9, 2, 7};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Before: ");
    printArray(arr, size);

    bubbleSort(arr, size);

    printf("After:  ");
    printArray(arr, size);

    return 0;
}
