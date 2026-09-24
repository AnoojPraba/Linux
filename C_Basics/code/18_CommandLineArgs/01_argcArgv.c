#include <stdio.h>

int main(int argc, char *argv[])
{
    int i;

    // argv[0] is the program name itself; argc counts it, so real arguments
    // start at argv[1].
    printf("argc = %d\n", argc);
    for (i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    return 0;
}
