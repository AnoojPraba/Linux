# 62_LinkerAndLoaderMechanics

Linker and loader mechanics: how globals are split across ELF sections (.data, .bss, .text) and resolved across translation units, plus embedded linker scripts.

## Files
- `01_main.c` - main() using externs defined in another TU and calling printSections()
- `dataSections.c` - initialised vs uninitialised globals (.data vs .bss); helper TU with no main()
- `NOTES.md` - linker and loader notes; embedded linker scripts with explicit memory regions

## Build and run
- Two-TU program: `gcc -Wall -Wextra -std=gnu11 01_main.c dataSections.c -o /tmp/x && /tmp/x`, then inspect with `size /tmp/x`, `nm /tmp/x`, `readelf -S /tmp/x`.
- The Makefile has an explicit rule for `bin/62_LinkerAndLoaderMechanics/01_main`; `dataSections.c` is excluded from the generic rule.
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/62_LinkerAndLoaderMechanics/` (git-ignored).

## Key concepts / interview angles
- Initialised globals live in .data (stored in the file), zero/uninitialised in .bss (only a size), constants in .rodata, code in .text.
- Linker steps: symbol resolution, relocation, section merging; loader maps segments and (for dynamic ELF) invokes ld.so.
- Duplicate strong symbols fail to link; weak symbols and `-fcommon` history.
- Embedded: linker scripts define FLASH/RAM regions, copy .data from flash and zero .bss at start-up.

## Related
- `../63_DynamicLoading`
- `../64_StaticAndSharedLibraries`
- `../../../C_Basics/code/29_CompilerPipelineWalkthrough`
- `../01_BootProcess`
- `../../../C_Basics/code/30_MultiFile`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
