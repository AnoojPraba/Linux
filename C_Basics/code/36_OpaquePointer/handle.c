#include <stdlib.h>
#include "handle.h"

// The real definition, only visible inside this translation unit - a
// caller including handle.h has no way to know this struct even has a
// field called "value", let alone access it directly as handle->value.
struct Handle
{
    int value;
};

Handle *handleCreate(int initialValue)
{
    Handle *handle = malloc(sizeof(Handle));

    handle->value = initialValue;
    return handle;
}

void handleDestroy(Handle *handle)
{
    free(handle);
}

int handleGetValue(const Handle *handle)
{
    return handle->value;
}

void handleSetValue(Handle *handle, int value)
{
    // A real accessor could validate/clamp here - something a raw public
    // struct field could never enforce, since callers could always just
    // assign to it directly.
    handle->value = value;
}
