#include <stdio.h>

void swap(int *a, int *b)
{
    int tmp = *a;

    *a = *b;
    *b = tmp;
}

// Sift the element at index down until the max-heap property holds for
// the subtree rooted there - "heapSize" is the logical heap size, which
// shrinks as heapSort extracts elements from the end of the array.
void siftDown(int arr[], int heapSize, int index)
{
    int largest = index;
    int left = (2 * index) + 1;
    int right = (2 * index) + 2;

    if ((left < heapSize) && (arr[left] > arr[largest]))
    {
        largest = left;
    }
    if ((right < heapSize) && (arr[right] > arr[largest]))
    {
        largest = right;
    }
    if (largest != index)
    {
        swap(&arr[index], &arr[largest]);
        siftDown(arr, heapSize, largest);
    }
}

// Heap sort: build a max-heap in place (O(n)), then repeatedly swap the
// root (the current max) to the end of the shrinking heap and re-sift
// (O(log n) each) - O(n log n) overall, in place, no extra array needed.
void heapSort(int arr[], int size)
{
    int i;

    for (i = (size / 2) - 1; i >= 0; i--)
    {
        siftDown(arr, size, i);
    }

    for (i = size - 1; i > 0; i--)
    {
        swap(&arr[0], &arr[i]);
        siftDown(arr, i, 0);
    }
}

int main()
{
    int arr[] = {5, 3, 8, 1, 9, 2, 7};
    int size = sizeof(arr) / sizeof(arr[0]);
    int i;

    heapSort(arr, size);

    printf("sorted: ");
    for (i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
