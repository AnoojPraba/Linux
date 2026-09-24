#include <stdio.h>

// A file-scope static restricts linkage to this translation unit only -
// other .c files cannot reference it even with a matching extern declaration.
static int internalCounter = 0;

static void increment(void)
{
    internalCounter++;
}

int main()
{
    increment();
    increment();
    increment();
    printf("internalCounter = %d\n", internalCounter);

    return 0;
}
