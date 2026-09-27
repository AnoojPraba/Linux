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
  running `size code/bin/61_LinkerAndLoaderMechanics/01_main` shows nonzero
  `.data`/`.bss` sizes, and `nm code/bin/61_LinkerAndLoaderMechanics/01_main`
  shows both symbols with different section type letters (`D` for `.data`,
  `B` for `.bss`).
- See `62_DynamicLoading` for `dlopen()`/`dlsym()` runtime loading of a
  shared library, and `../C_Basics/code/30_MultiFile`/`../C_Basics/code/64_ExternC`
  for other multi-TU linking patterns already in this repo.

## Embedded linker scripts: explicit memory regions

- Everything above describes the desktop/hosted-OS case: the OS loader picks
  the load addresses and hands off to the dynamic linker. On bare-metal
  embedded firmware (or firmware on a minimal RTOS) there is no OS loader -
  the linker script (a `.ld` file) must explicitly place every section into a
  specific physical memory region, because the target only has fixed,
  known-in-advance physical memory (a FLASH region for code and a RAM region
  for data), not a virtual address space an OS can arrange freely.
- **`MEMORY` block** declares the physical regions and their attributes:
  ```
  MEMORY
  {
      FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
      RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
  }
  ```
  `rx`/`rwx` are access permissions (readable/executable, readable/writable/
  executable); `ORIGIN`/`LENGTH` fix where the region starts and how big it is.
- **`SECTIONS` block** maps ELF sections into those regions, e.g.:
  ```
  SECTIONS
  {
      .text   : { *(.text) }   > FLASH
      .rodata : { *(.rodata) } > FLASH
      .data   : { *(.data) }   > RAM AT> FLASH
      .bss    : { *(.bss) }    > RAM
  }
  ```
  `.text`/`.rodata` load and run from FLASH (non-volatile, keeps its
  contents with power off). `.data` needs both: its *initial values* must
  live in FLASH (`AT> FLASH`, the load address) so they survive a power
  cycle, but the *running* variables must live in RAM (`> RAM`, the virtual/
  run address) since code reads/writes them at full RAM speed.
- **Startup code's job, before `main()` runs**: a microcontroller's
  startup/reset handler (not an OS loader - there is no OS yet) walks the
  symbols the linker script emits for each section's load/run addresses and:
  1. Copies `.data`'s initial values from their FLASH load address to their
     RAM run address (RAM contents are lost across power-off, so the only
     place the initial values can persist is FLASH).
  2. Zero-fills `.bss` in RAM (matching the `.bss` no-file-space convention
     from the ELF section list above, just done by hand instead of by an
     OS's page-fault-driven zero-fill-on-demand).
  3. Calls `main()`.
  This is the bare-metal analogue of what a hosted OS's loader/dynamic
  linker does automatically at `exec()` time (see the "Static vs dynamic
  linking" section above) - on a microcontroller there is no OS process
  loader to do it, so the startup code does it explicitly, in a few lines of
  assembly/C run straight out of reset.
- Physical-memory-region placement here is the same theme as
  `28_MemoryAddressingAndFragmentation`/`29_MemoryManagement`, but at the
  opposite end of the spectrum: those folders cover an OS giving each
  process its own virtual address space over physical RAM it manages
  dynamically, while an embedded linker script instead pins every section to
  a fixed physical address at link time, with no MMU/paging layer in between
  on many microcontrollers.
