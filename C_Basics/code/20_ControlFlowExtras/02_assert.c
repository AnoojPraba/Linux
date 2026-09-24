#include <stdio.h>
#include <assert.h>

// assert() checks an invariant that should be impossible to violate if the
// rest of the program is correct - not a substitute for validating actual
// user input, which can legitimately be wrong and needs a real error path
// instead of a crash.
int divide(int numerator, int denominator)
{
    assert(denominator != 0);
    return numerator / denominator;
}

int main()
{
    printf("divide(10, 2) = %d\n", divide(10, 2));
    printf("divide(9, 3) = %d\n", divide(9, 3));

    // Defining NDEBUG before including <assert.h> compiles every assert()
    // out entirely (they become no-ops) - so an assert must never contain a
    // side effect the program depends on, e.g. assert(x = compute()).
    printf("all assertions passed\n");

    return 0;
}
