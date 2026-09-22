#include <stdio.h>
#include "libgreet.h"

// Linked against libgreet.so (shared library) at link time, but the actual
// code is resolved at load time (via the dynamic linker/loader, ld.so) -
// contrast this ordinary link-time-declared dependency with
// 74_DynamicLoading, where the .so is instead opened explicitly at runtime
// via dlopen()/dlsym() and the program has no link-time dependency on it
// at all. Run with LD_LIBRARY_PATH set to this folder's bin dir (or after
// installing libgreet.so somewhere the loader searches) - see NOTES.md.
int main()
{
    greet("shared demo");
    printf("addNumbers(4, 5) = %d\n", addNumbers(4, 5));
    return 0;
}
