#include <stdio.h>

int linearSearch(int arr[], int size, int target)
{
    int i;

    // O(n): checks every element until a match or the end of the array -
    // no assumption about ordering, unlike binary search.
    for (i = 0; i < size; i++)
    {
        if (arr[i] == target)
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    int arr[] = {5, 3, 8, 1, 9, 2};
    int size = sizeof(arr) / sizeof(arr[0]);
    int target = 9;
    int result = linearSearch(arr, size, target);

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
