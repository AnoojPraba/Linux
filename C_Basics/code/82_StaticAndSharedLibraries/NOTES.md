# Static and Shared Libraries

`libgreet.c`/`libgreet.h` is built two ways from the exact same source, both
via the Makefile (`make` from `code/`, or run the commands below by hand):
- **Static**: `01_useStatic` links against `libgreet.a`.
- **Shared**: `02_useShared` links against `libgreet.so`.

Related but distinct folders: `74_DynamicLoading` also produces a `.so`, but
loads it explicitly at *runtime* via `dlopen()`/`dlsym()` with no link-time
dependency at all - a plugin-style pattern. This folder is the far more
common case: an ordinary link-time dependency on a shared library, resolved
automatically by the dynamic linker (`ld.so`) at process startup.
`75_ExternC` is about a different axis entirely (C/C++ linkage naming, not
static-vs-shared) - also unrelated here beyond both involving multi-TU links.

## Building by hand

```sh
# Static
gcc -c libgreet.c -o libgreet.o
ar rcs libgreet.a libgreet.o
gcc 01_useStatic.c libgreet.a -o useStatic

# Shared
gcc -fPIC -shared libgreet.c -o libgreet.so
gcc 02_useShared.c -L. -lgreet -Wl,-rpath,'$ORIGIN' -o useShared
```

## .a (static) vs .so (shared)

- **Link-time vs load-time resolution**: a static archive's object code is
  copied directly into the final binary at link time - the result is a
  single self-contained executable with no runtime dependency on the
  library. A shared library's code stays in a separate `.so` file; the
  dynamic linker resolves and maps it in at process startup (or, for
  `dlopen()`, at an arbitrary later point - see `74_DynamicLoading`).
- **Binary size**: static linking duplicates the library's code into every
  binary that links it (bigger executables, more disk/memory if many
  processes each embed a copy); shared linking keeps one on-disk copy that
  multiple processes can also share in memory (the OS maps the same
  physical pages read-only into each process).
- **Upgrade/patch story**: a shared library can be upgraded in place (drop
  in a new `libgreet.so` with the same SONAME) and every process that loads
  it afterward picks up the fix with no rebuild - this is how most distro
  security patches for common libraries (libc, openssl) are delivered.
  Static linking means every binary using the old code must be individually
  rebuilt and redeployed to pick up a fix.
- **Startup cost / dependency risk**: shared libraries add a small load-time
  resolution cost and introduce a runtime dependency (the `.so` must be
  found at the versions the binary expects - "DLL hell"/`.so` version
  mismatches); static binaries have neither, at the cost of the size/patch
  tradeoffs above.
- **Symbol versioning basics**: a shared library's SONAME (e.g.
  `libgreet.so.1`) plus embedded per-symbol version tags (via a linker
  version script) let a `.so` evolve while old binaries linked against an
  older symbol version keep resolving to that exact old implementation -
  this is how glibc ships multiple ABI-compatible versions of the same
  function name (e.g. `memcpy@GLIBC_2.2.5` vs a newer default). Static
  linking has no equivalent concept - there's only ever one copy, chosen at
  link time.

## Inspecting shared libraries

- `ldd ./useShared` lists the shared libraries a dynamically-linked binary
  depends on and where each resolves from (or "not found" if missing).
- `nm -D libgreet.so` lists dynamic symbols; `nm libgreet.a` lists the
  archive members' symbols (per `.o` inside it).
- `objdump -T libgreet.so` is another view of the dynamic symbol table
  (similar to `nm -D`), showing which symbols are exported/imported and
  their versions if any are defined.
- `file libgreet.a` vs `file libgreet.so` - confirms archive vs
  shared-object ELF type at a glance.
