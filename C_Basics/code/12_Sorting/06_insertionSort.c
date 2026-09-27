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

// Grow a sorted prefix one element at a time: take the next element and
// shift larger elements in the sorted prefix rightward to make room for it.
void insertionSort(int arr[], int size)
{
    int i;

    for (i = 1; i < size; i++)
    {
        int key = arr[i];
        int j = i - 1;

        while ((j >= 0) && (arr[j] > key))
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int main()
{
    int arr[] = {8, 3, 5, 1, 9, 2, 7};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Before: ");
    printArray(arr, size);

    insertionSort(arr, size);

    printf("After:  ");
    printArray(arr, size);

    return 0;
}
