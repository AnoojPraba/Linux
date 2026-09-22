#include <stdio.h>
#include "handle.h"

int main()
{
    Handle *handle = handleCreate(10);

    printf("initial value = %d\n", handleGetValue(handle));

    handleSetValue(handle, 42);
    printf("after set, value = %d\n", handleGetValue(handle));

    // handle->value would be a compile error here - Handle is an
    // incomplete type in this translation unit, so the compiler doesn't
    // know it has a "value" member at all, let alone its offset.

    handleDestroy(handle);
    return 0;
}
