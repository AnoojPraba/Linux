# Linker and Loader Mechanics

- **Compilation pipeline**: preprocess -> compile -> assemble (produces a
  relocatable object `.o` with unresolved external symbols) -> link
  (resolves symbols across object files/libraries, produces an executable or
  shared object).
- **Symbol resolution**: the linker matches each undefined symbol reference
  in one `.o` against a definition in another `.o` or library. Multiple
  definitions of the same strong symbol = link error; unresolved symbol with
  no definition anywhere = link error.
- **Key ELF sections**:
  - `.text` - executable code (read-only, shared across processes running
    the same binary).
  - `.rodata` - read-only data: string literals, `const` globals.
  - `.data` - initialized global/static variables with a nonzero value
    (stored in the file, copied into memory at load time).
  - `.bss` - uninitialized (or zero-initialized) global/static variables;
    only a size is stored in the file, zero-filled pages at load time - no
    file space cost.
- **Static vs dynamic linking**:
  - Static (`.a` archives): library code copied into the final executable at
    link time. Bigger binary, no runtime dependency, no shared-library
    version skew, but every binary must be rebuilt to pick up a library fix.
  - Dynamic (`.so` shared objects): resolved at load time (or lazily, at
    first call, via the PLT/GOT) by the dynamic loader (`ld.so`). Smaller
    binaries, shared code pages across processes, library updates apply
    without rebuilding, but introduces runtime dependency/version risk
    ("DLL hell").
- **This folder's demo**: `dataSections.c` defines `initializedGlobal` (goes
  to `.data`) and `uninitializedGlobal` (goes to `.bss`). After building,
  running `size code/bin/73_LinkerAndLoaderMechanics/01_main` shows nonzero
  `.data`/`.bss` sizes, and `nm code/bin/73_LinkerAndLoaderMechanics/01_main`
  shows both symbols with different section type letters (`D` for `.data`,
  `B` for `.bss`).
- See `74_DynamicLoading` for `dlopen()`/`dlsym()` runtime loading of a
  shared library, and `27_MultiFile`/`75_ExternC` for other multi-TU linking
  patterns already in this repo.
