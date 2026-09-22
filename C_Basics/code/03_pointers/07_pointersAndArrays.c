#include <stdio.h>

void printSize(int arr[])
{
    // Inside a function, an array parameter has already decayed to a
    // pointer - sizeof(arr) here is sizeof(int *), not the original array's
    // byte size, no matter what the caller passed.
    printf("sizeof(arr) inside function = %lu\n", (unsigned long)sizeof(arr));
}

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};
    int *p = arr;

    // In most expressions, an array name decays to a pointer to its first
    // element - arr and &arr[0] hold the same address.
    printf("arr == &arr[0]: %d\n", arr == &arr[0]);
    printf("p == arr: %d\n", p == arr);

    printf("arr[2] = %d, *(arr + 2) = %d\n", arr[2], *(arr + 2));
    printf("p[2] = %d, *(p + 2) = %d\n\n", p[2], *(p + 2));

    // sizeof(arr) here still knows the full array size because arr is the
    // actual array in this scope, not a decayed parameter.
    printf("sizeof(arr) in main = %lu (whole array)\n", (unsigned long)sizeof(arr));
    printSize(arr);

    return 0;
}
