#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 8
#define OVERFLOW_LEN 16

int main()
{
    char *buffer = malloc(BUF_SIZE);

    if (buffer == NULL)
    {
        return 1;
    }

    // Deliberate heap buffer overflow: OVERFLOW_LEN bytes written into an
    // 8-byte allocation. Compiles cleanly (memset takes a plain size_t);
    // valgrind's default mode does not always report this exact pattern,
    // but ASan reports it precisely (see NOTES.md).
    memset(buffer, 'A', OVERFLOW_LEN);
    printf("wrote %d bytes into an %d-byte buffer\n", OVERFLOW_LEN, BUF_SIZE);

    free(buffer);
    return 0;
}
