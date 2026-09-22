#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 2;
    int count = 0;
    int *arr = malloc(capacity * sizeof(int));
    int i;

    for (i = 1; i <= 5; i++)
    {
        if (count == capacity)
        {
            capacity *= 2;
            // realloc grows/shrinks a block in place if possible, otherwise
            // it allocates a new block, copies the old contents, and frees the old
            // block - so always reassign the returned pointer, never assume the
            // original address stayed valid.
            arr = realloc(arr, capacity * sizeof(int));
            printf("grew capacity to %d\n", capacity);
        }
        arr[count++] = i * i;
    }

    printf("values: ");
    for (i = 0; i < count; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
