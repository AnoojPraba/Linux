#include <stdio.h>
#include <string.h>

// The strict aliasing rule says an object may only be accessed through a
// pointer of its own type (or a compatible/char type) - accessing the
// same memory through two pointers of unrelated types (here, int * and
// float *) is undefined behavior. __attribute__((noinline)) forces this
// to remain a real function call at every optimization level, so the
// compiler's aliasing assumptions about its two pointer parameters are
// what's actually on trial here, not whether it got inlined away.
__attribute__((noinline))
int violateAliasing(int *intPtr, float *floatPtr)
{
    *intPtr = 10;
    // The compiler is allowed to assume intPtr and floatPtr can never
    // point at the same memory, purely because their types differ - so
    // it may return the constant 10 it just wrote, without re-reading
    // memory, even though the very next line writes to that same address
    // through floatPtr.
    *floatPtr = 20.0f;
    return *intPtr;
}

// The correct way to reinterpret bytes between types: memcpy sidesteps
// aliasing entirely, since it's specified to operate on raw bytes and the
// compiler can't apply the same "these can't overlap" reasoning to it.
float reinterpretIntAsFloat(int value)
{
    float result;

    memcpy(&result, &value, sizeof(result));
    return result;
}

int main()
{
    int value = 0;
    float *aliasedFloatPtr = (float *)&value;
    int returnedValue;

    // aliasedFloatPtr and &value point at the exact same 4 bytes, just
    // through unrelated pointer types - this call is the actual UB.
    returnedValue = violateAliasing(&value, aliasedFloatPtr);

    printf("violateAliasing() returned: %d\n", returnedValue);
    printf("actual memory now holds (reread as int): %d\n", value);
    printf("(if these two differ, the compiler assumed the writes couldn't\n"
           " alias and returned a stale cached value instead of re-reading\n"
           " memory - try recompiling this file with -O2 vs -O0 to see the\n"
           " difference; this repo's own Makefile builds without -O2, so it\n"
           " won't show the divergence by default)\n\n");

    printf("safe reinterpretation via memcpy: %f\n", reinterpretIntAsFloat(1078530011));

    return 0;
}
