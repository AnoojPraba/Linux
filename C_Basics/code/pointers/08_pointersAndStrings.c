#include <stdio.h>

int main()
{
    // A char array: the characters are copied into modifiable stack memory.
    char arr[] = "hello";

    // A char pointer to a string literal: literal storage is implementation
    // defined and typically read-only - writing through str is undefined
    // behavior, unlike writing through arr.
    char *str = "world";
    int i;

    arr[0] = 'H';
    printf("arr = %s\n", arr);

    // str++ is legal - it just moves the pointer to point further into the
    // literal - but *str = 'W' would not be, since the literal itself is
    // not writable.
    printf("str = %s\n", str);
    printf("*(str + 1) = %c\n\n", *(str + 1));

    // Walking a string via pointer, relying on the '\0' terminator instead
    // of a known length.
    char *walker = arr;

    printf("walking arr: ");
    while (*walker != '\0')
    {
        printf("%c", *walker);
        walker++;
    }
    printf("\n\n");

    // An array of char pointers - each element points at a different
    // string literal, not at a 2D block of characters.
    char *names[] = {"Alice", "Bob", "Carol"};

    for (i = 0; i < 3; i++)
    {
        printf("names[%d] = %s\n", i, names[i]);
    }

    return 0;
}
