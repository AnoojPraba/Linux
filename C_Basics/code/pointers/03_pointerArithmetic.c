#include <stdio.h>

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};
    int *p = arr;
    int i;

    for (i = 0; i < 5; i++)
    {
        printf("arr[%d] = %d, address = %p\n", i, *(p + i), (p + i));
    }

    // p++ moves by sizeof(int) bytes, not by 1 byte
    p++;
    printf("\nAfter p++, *p = %d (was arr[1])\n", *p);

    p += 2;
    printf("After p += 2, *p = %d (now arr[3])\n", *p);

    printf("\nDifference between &arr[4] and &arr[0] = %ld elements\n",
           (long)(&arr[4] - &arr[0]));

    return 0;
}
