#include <stdio.h>
#include <dlfcn.h>

#define PLUGIN_PATH "./libplugin.so"

int main()
{
    // dlopen loads a shared library at runtime, by path, instead of it
    // being resolved once at program startup like a normally linked
    // library - this is what makes a real plugin system possible: the
    // set of loadable modules doesn't have to be known when this program
    // was compiled.
    void *handle = dlopen(PLUGIN_PATH, RTLD_NOW);
    int (*greetFunc)(const char *);
    void *internalHelper;

    if (handle == NULL)
    {
        printf("dlopen failed: %s\n", dlerror());
        return 1;
    }

    // dlsym looks up a symbol by name string at runtime - the function
    // pointer's type has to be supplied by the caller (there's no type
    // information carried in the library itself), so a mismatched cast
    // here would compile fine but crash or misbehave at the call below.
    greetFunc = (int (*)(const char *))dlsym(handle, "pluginGreet");
    if (greetFunc == NULL)
    {
        printf("dlsym(pluginGreet) failed: %s\n", dlerror());
        return 1;
    }

    printf("pluginGreet returned: %d\n", greetFunc("world"));

    // pluginInternalHelper was compiled without default visibility (see
    // plugin.c), so this lookup is expected to fail even though the
    // function exists in the library's own object code - visibility
    // controls what's reachable from outside, not what's present.
    internalHelper = dlsym(handle, "pluginInternalHelper");
    if (internalHelper == NULL)
    {
        printf("dlsym(pluginInternalHelper) failed as expected: %s\n", dlerror());
    }

    dlclose(handle);
    return 0;
}
