#include <stdio.h>

#define MAX_MERGE_SIZE 128

void printArray(int arr[], int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Merge two already-sorted runs arr[low..mid] and arr[mid+1..high] into a
// single sorted run, using a temporary buffer since the merge can't be done
// in place without overwriting elements still needed for comparison.
void merge(int arr[], int low, int mid, int high)
{
    int temp[MAX_MERGE_SIZE];
    int i = low;
    int j = mid + 1;
    int k = low;

    while ((i <= mid) && (j <= high))
    {
        if (arr[i] <= arr[j])
        {
            temp[k++] = arr[i++];
        }
        else
        {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid)
    {
        temp[k++] = arr[i++];
    }
    while (j <= high)
    {
        temp[k++] = arr[j++];
    }
    for (i = low; i <= high; i++)
    {
        arr[i] = temp[i];
    }
}

void mergeSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int mid = low + (high - low) / 2;

        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
}

int main()
{
    int arr[] = {8, 3, 5, 1, 9, 2, 7};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Before: ");
    printArray(arr, size);

    mergeSort(arr, 0, size - 1);

    printf("After:  ");
    printArray(arr, size);

    return 0;
}
