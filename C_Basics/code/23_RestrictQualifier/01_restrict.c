#include <stdio.h>

// restrict is a promise to the compiler: for the duration of this
// function, the memory pointed to by "dest" and the memory pointed to by
// "src" never overlap. The compiler can then skip re-reading src after
// every write to dest (it "knows" a write to dest can't have changed src),
// which is exactly the kind of optimization memcpy relies on - and exactly
// why memcpy's behavior is undefined if the buffers do overlap (that's
// what memmove is for).
void copyArray(int *restrict dest, const int *restrict src, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }
}

int main()
{
    int src[5] = {1, 2, 3, 4, 5};
    int dest[5];
    int i;

    copyArray(dest, src, 5);

    printf("dest: ");
    for (i = 0; i < 5; i++)
    {
        printf("%d ", dest[i]);
    }
    printf("\n");

    return 0;
}
