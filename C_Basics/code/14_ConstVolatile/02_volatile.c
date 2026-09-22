#include <stdio.h>

int main()
{
    // volatile tells the compiler this value may change outside the normal
    // flow of the program (a hardware register, a signal handler, another
    // thread) - so it must re-read it from memory every time, never cache
    // it in a register or optimize the read away.
    volatile int flag = 0;

    printf("flag before = %d\n", flag);
    flag = 1;
    printf("flag after = %d\n", flag);

    return 0;
}
