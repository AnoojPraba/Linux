#include <stdio.h>

int binarySearchIterative(int arr[], int size, int target)
{
    int low = 0;
    int high = size - 1;

    // O(log n): requires a sorted array. Each iteration halves the search
    // range instead of walking one element at a time.
    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (arr[mid] == target)
        {
            return mid;
        }
        else if (arr[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return -1;
}

int main()
{
    int arr[] = {2, 4, 6, 8, 10, 12, 14};
    int size = sizeof(arr) / sizeof(arr[0]);
    int target = 12;
    int result = binarySearchIterative(arr, size, target);

    if (result != -1)
    {
        printf("Found %d at index %d\n", target, result);
    }
    else
    {
        printf("%d not found\n", target);
    }

    return 0;
}
