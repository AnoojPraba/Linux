#include <stdio.h>
#include <stdlib.h>

// qsort's comparator receives two const void * - it must cast them to the
// actual element type before comparing. Return negative if a < b, 0 if
// equal, positive if a > b - the same contract as strcmp.
int compareInts(const void *a, const void *b)
{
    int intA = *(const int *)a;
    int intB = *(const int *)b;

    return intA - intB;
}

int main()
{
    int arr[] = {5, 3, 8, 1, 9, 2};
    int size = sizeof(arr) / sizeof(arr[0]);
    int i;

    // qsort is a generic sort: it only needs the array, element count,
    // element size, and a comparator - it has no idea what type it's
    // sorting, which is why the comparator is where type-specific
    // comparison logic lives (see 06_voidPointer.c for the same void *
    // type-erasure idea).
    qsort(arr, size, sizeof(int), compareInts);

    printf("sorted: ");
    for (i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
