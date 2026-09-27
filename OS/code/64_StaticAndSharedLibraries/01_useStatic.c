#include <stdio.h>
#include "libgreet.h"

// Linked against libgreet.a (static archive) - see NOTES.md and the
// Makefile rule for the exact ar/gcc commands. The archive's object code
// is copied into this binary at link time, so it runs with no dependency
// on libgreet.so/.a existing at runtime.
int main()
{
    greet("static demo");
    printf("addNumbers(2, 3) = %d\n", addNumbers(2, 3));
    return 0;
}
