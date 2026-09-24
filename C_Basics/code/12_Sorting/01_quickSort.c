#include <stdio.h>

void swap(int *a, int *b)
{
    int tmp = *a;

    *a = *b;
    *b = tmp;
}

void printArray(int arr[], int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Lomuto partition: picks the last element as pivot and walks the range,
// swapping every element smaller than the pivot into the low side. Returns
// the pivot's final resting index, which splits the array into two halves.
int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;
    int j;

    for (j = low; j < high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(arr, low, high);

        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

int main()
{
    int arr[] = {8, 3, 5, 1, 9, 2, 7};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Before: ");
    printArray(arr, size);

    quickSort(arr, 0, size - 1);

    printf("After:  ");
    printArray(arr, size);

    return 0;
}
