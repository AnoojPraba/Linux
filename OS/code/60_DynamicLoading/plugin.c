#include <stdio.h>

// __attribute__((visibility("default"))) explicitly marks this symbol as
// exported from the shared library - relevant when the library is built
// with -fvisibility=hidden (see the Makefile rule for this plugin), which
// makes every symbol hidden by default unless marked otherwise. Hiding
// symbols by default is common practice for real shared libraries: it
// shrinks the exported symbol table, speeds up dynamic linking, and
// prevents accidental use of internal-only functions by callers.
__attribute__((visibility("default")))
int pluginGreet(const char *name)
{
    printf("plugin: hello, %s!\n", name);
    return 42;
}

// No visibility attribute - stays hidden when built with
// -fvisibility=hidden, so it cannot be dlsym()'d or linked against from
// outside this shared library, even though it's a perfectly ordinary,
// non-static function.
int pluginInternalHelper(void)
{
    return 7;
}
