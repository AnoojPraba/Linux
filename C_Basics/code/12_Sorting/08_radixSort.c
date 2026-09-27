#include <stdio.h>

#define DIGIT_BASE 10

void printArray(int arr[], int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int getMax(int arr[], int size)
{
    int max = arr[0];
    int i;

    for (i = 1; i < size; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}

// Stable counting sort keyed on the digit at "place" (1, 10, 100, ...),
// used as the subroutine radixSort calls once per digit.
void countingSortByDigit(int arr[], int size, int place)
{
    int output[size];
    int count[DIGIT_BASE] = {0};
    int i;

    for (i = 0; i < size; i++)
    {
        int digit = (arr[i] / place) % DIGIT_BASE;

        count[digit]++;
    }
    for (i = 1; i < DIGIT_BASE; i++)
    {
        count[i] += count[i - 1];
    }
    for (i = size - 1; i >= 0; i--)
    {
        int digit = (arr[i] / place) % DIGIT_BASE;

        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }
    for (i = 0; i < size; i++)
    {
        arr[i] = output[i];
    }
}

// LSD radix sort: repeatedly sort by each digit, from least to most
// significant, using the digit-wise counting sort above.
void radixSort(int arr[], int size)
{
    int max = getMax(arr, size);
    int place;

    for (place = 1; max / place > 0; place *= DIGIT_BASE)
    {
        countingSortByDigit(arr, size, place);
    }
}

int main()
{
    int arr[] = {170, 45, 75, 90, 802, 24, 2, 66};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Before: ");
    printArray(arr, size);

    radixSort(arr, size);

    printf("After:  ");
    printArray(arr, size);

    return 0;
}
