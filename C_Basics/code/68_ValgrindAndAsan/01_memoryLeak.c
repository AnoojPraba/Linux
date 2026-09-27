#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 64

/*****************************************************************************
 * Name: allocateBuffer
 *
 * Description:
 *         Allocates a buffer and fills it, but the caller never frees it -
 *         deliberately, so this is a clean leak for valgrind/ASan to catch.
 *
 * Returns:
 *         Pointer to a heap-allocated, never-freed buffer.
 *****************************************************************************/
char *allocateBuffer(void)
{
    char *buffer = malloc(BUF_SIZE);

    if (buffer == NULL)
    {
        return NULL;
    }
    snprintf(buffer, BUF_SIZE, "leaked buffer");
    return buffer;
}

int main()
{
    char *buffer = allocateBuffer();

    if (buffer != NULL)
    {
        printf("%s\n", buffer);
    }

    // No free(buffer) here - deliberate leak.
    return 0;
}
