#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 32

int main()
{
    char *buffer = malloc(BUF_SIZE);
    char firstChar;

    if (buffer == NULL)
    {
        return 1;
    }
    snprintf(buffer, BUF_SIZE, "hello");

    free(buffer);

    // Deliberate use-after-free: buffer is dangling here. Compiles fine
    // (the compiler can't generally prove this is invalid); valgrind and
    // ASan both catch the read at runtime.
    firstChar = buffer[0];
    printf("first char after free: %c\n", firstChar);

    return 0;
}
