#include <stdio.h>

int ternarySearch(int arr[], int low, int high, int target)
{
    // Splits the range into three parts each iteration (two comparisons per
    // level) instead of the two parts binary search uses (one comparison
    // per level).
    while (low <= high)
    {
        int third = (high - low) / 3;
        int mid1 = low + third;
        int mid2 = high - third;

        if (arr[mid1] == target)
        {
            return mid1;
        }
        if (arr[mid2] == target)
        {
            return mid2;
        }

        if (target < arr[mid1])
        {
            high = mid1 - 1;
        }
        else if (target > arr[mid2])
        {
            low = mid2 + 1;
        }
        else
        {
            low = mid1 + 1;
            high = mid2 - 1;
        }
    }
    return -1;
}

int main()
{
    int arr[] = {2, 4, 6, 8, 10, 12, 14, 16, 18};
    int size = sizeof(arr) / sizeof(arr[0]);
    int target = 14;
    int result;

    printf("searching for %d in array of size %d\n", target, size);
    result = ternarySearch(arr, 0, size - 1, target);

    if (result != -1)
    {
        printf("found %d at index %d\n", target, result);
    }
    else
    {
        printf("%d not found\n", target);
    }

    return 0;
}
