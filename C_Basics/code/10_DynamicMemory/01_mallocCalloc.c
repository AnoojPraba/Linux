#include <stdio.h>
#include <stdlib.h>

int main()
{
    int size = 5;
    int i;

    // malloc: allocates raw, uninitialized memory
    int *a = malloc(size * sizeof(int));

    for (i = 0; i < size; i++)
    {
        a[i] = i + 1;
    }
    printf("malloc'd array: ");
    for (i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    free(a);

    // calloc: allocates and zero-initializes memory
    int *b = calloc(size, sizeof(int));

    printf("calloc'd array (zero-initialized): ");
    for (i = 0; i < size; i++)
    {
        printf("%d ", b[i]);
    }
    printf("\n");
    free(b);

    return 0;
}
