#include <stdio.h>

#define KEY_RANGE 100

void printArray(int arr[], int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Count occurrences of each key in the known range [0, KEY_RANGE), then turn
// those counts into prefix sums giving each key's final position, and place
// elements back-to-front to keep the sort stable.
void countingSort(int arr[], int size)
{
    int count[KEY_RANGE] = {0};
    int output[KEY_RANGE];
    int i;

    for (i = 0; i < size; i++)
    {
        count[arr[i]]++;
    }
    for (i = 1; i < KEY_RANGE; i++)
    {
        count[i] += count[i - 1];
    }
    for (i = size - 1; i >= 0; i--)
    {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }
    for (i = 0; i < size; i++)
    {
        arr[i] = output[i];
    }
}

int main()
{
    int arr[] = {28, 3, 55, 1, 90, 2, 71, 3};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Before: ");
    printArray(arr, size);

    countingSort(arr, size);

    printf("After:  ");
    printArray(arr, size);

    return 0;
}
