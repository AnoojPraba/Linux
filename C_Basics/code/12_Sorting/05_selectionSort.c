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

// For each position, scan the remaining unsorted region for the minimum and
// swap it into place. Unlike bubble sort, only one swap happens per pass.
void selectionSort(int arr[], int size)
{
    int i;
    int j;

    for (i = 0; i < size - 1; i++)
    {
        int minIdx = i;

        for (j = i + 1; j < size; j++)
        {
            if (arr[j] < arr[minIdx])
            {
                minIdx = j;
            }
        }
        if (minIdx != i)
        {
            int tmp = arr[i];

            arr[i] = arr[minIdx];
            arr[minIdx] = tmp;
        }
    }
}

int main()
{
    int arr[] = {8, 3, 5, 1, 9, 2, 7};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Before: ");
    printArray(arr, size);

    selectionSort(arr, size);

    printf("After:  ");
    printArray(arr, size);

    return 0;
}
