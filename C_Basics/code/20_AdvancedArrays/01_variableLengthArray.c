#include <stdio.h>

void printRow(int size, int row[size])
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("%d ", row[i]);
    }
    printf("\n");
}

int main()
{
    int n = 5;

    // A VLA's size is a runtime value, not a compile-time constant - the
    // array is still allocated on the stack, not the heap, so it's freed
    // automatically when it goes out of scope, unlike malloc'd memory.
    // Unlike malloc, there's no way to check whether the allocation
    // succeeded - a VLA that's too large simply overflows the stack.
    int vla[n];
    int i;

    for (i = 0; i < n; i++)
    {
        vla[i] = i * i;
    }

    printf("VLA contents: ");
    printRow(n, vla);

    return 0;
}
