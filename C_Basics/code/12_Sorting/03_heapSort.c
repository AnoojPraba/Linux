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

// Restore the max-heap property for the subtree rooted at "root", assuming
// its children are already valid heaps. "limit" is the logical heap size,
// which shrinks as heapSort pops elements off the end of the array.
void heapify(int arr[], int limit, int root)
{
    int largest = root;
    int left = (2 * root) + 1;
    int right = (2 * root) + 2;

    if ((left < limit) && (arr[left] > arr[largest]))
    {
        largest = left;
    }
    if ((right < limit) && (arr[right] > arr[largest]))
    {
        largest = right;
    }
    if (largest != root)
    {
        int tmp = arr[root];

        arr[root] = arr[largest];
        arr[largest] = tmp;
        heapify(arr, limit, largest);
    }
}

void heapSort(int arr[], int size)
{
    int i;

    // Build a max-heap in place: start at the last parent node and heapify
    // backwards to the root, so every subtree is a valid heap by the end.
    for (i = (size / 2) - 1; i >= 0; i--)
    {
        heapify(arr, size, i);
    }

    // Repeatedly swap the max (root) to the end of the shrinking heap
    // region, then re-heapify the reduced heap.
    for (i = size - 1; i > 0; i--)
    {
        int tmp = arr[0];

        arr[0] = arr[i];
        arr[i] = tmp;
        heapify(arr, i, 0);
    }
}

int main()
{
    int arr[] = {8, 3, 5, 1, 9, 2, 7};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Before: ");
    printArray(arr, size);

    heapSort(arr, size);

    printf("After:  ");
    printArray(arr, size);

    return 0;
}
