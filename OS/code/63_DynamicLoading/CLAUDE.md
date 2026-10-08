# 63_DynamicLoading

Runtime plugin loading with dlopen/dlsym/dlclose from a shared library built with hidden default visibility.

## Files
- `01_loader.c` - dlopen("./libplugin.so", RTLD_NOW), dlsym a function, call it, dlclose
- `plugin.c` - plugin source (no main()); only symbols marked visibility("default") are exported

## Build and run
- Manual: `gcc -Wall -fPIC -fvisibility=hidden -shared plugin.c -o libplugin.so && gcc -Wall -Wextra -std=gnu11 01_loader.c -o loader -ldl && ./loader` (the loader opens `./libplugin.so`, so run it from the directory that holds the .so).
- `make` from `..` builds `bin/63_DynamicLoading/libplugin.so` and `bin/63_DynamicLoading/01_loader`; run the loader from `../../bin/63_DynamicLoading`. `-ldl` is explicit in the Makefile (glibc >= 2.34, as here, has dlopen in libc, older systems need it).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/63_DynamicLoading/` (git-ignored).

## Key concepts / interview angles
- `dlopen` flags: RTLD_NOW (resolve immediately) vs RTLD_LAZY, RTLD_GLOBAL vs RTLD_LOCAL.
- `-fvisibility=hidden` plus explicit exports keeps the plugin ABI small.
- Always check `dlerror()`; function pointers from `dlsym` need a cast.
- Plugin pitfalls: ABI/version mismatch, unloading while code is running, static state duplication.

## Gotchas
- Fails with "dlopen failed" if run from a directory without `libplugin.so`.

## Related
- `../64_StaticAndSharedLibraries`
- `../62_LinkerAndLoaderMechanics`
- `../../../C_Basics/code/64_ExternC`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
