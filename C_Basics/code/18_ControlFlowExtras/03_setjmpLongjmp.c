#include <stdio.h>
#include <setjmp.h>

jmp_buf recoveryPoint;

// longjmp unwinds the call stack straight back to the matching setjmp call,
// skipping every intervening return - the closest C gets to exceptions,
// with none of C++'s automatic destructor cleanup along the way.
void riskyOperation(int failCode)
{
    printf("riskyOperation: about to fail with code %d\n", failCode);
    longjmp(recoveryPoint, failCode);
    printf("this line never runs\n");
}

int main()
{
    // setjmp returns 0 the first time (falling through normally); a
    // longjmp() to this point later makes setjmp "return again", this time
    // with the value passed to longjmp.
    int result = setjmp(recoveryPoint);

    if (result == 0)
    {
        printf("first pass through setjmp, calling riskyOperation\n");
        riskyOperation(42);
        printf("this line never runs either\n");
    }
    else
    {
        printf("recovered via longjmp with code %d\n", result);
    }

    return 0;
}
