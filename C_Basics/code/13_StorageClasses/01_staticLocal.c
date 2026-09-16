#include <stdio.h>

// A static local variable is initialized once and persists across calls,
// unlike an ordinary local which is reinitialized every time the function runs.
int nextId(void)
{
    static int id = 0;

    id++;
    return id;
}

int main()
{
    int i;

    for (i = 0; i < 3; i++)
    {
        printf("nextId() = %d\n", nextId());
    }

    return 0;
}
