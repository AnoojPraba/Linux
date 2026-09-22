# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository purpose

This is a personal C learning/practice repository — a collection of small, standalone C
programs exploring core language concepts (loops, functions, arrays, search/sort
algorithms, pattern printing, pointers, bit manipulation, variable scope/linkage, tokens,
escape sequences, etc.). There is no test suite or package manifest. `code/` has a
Makefile for bulk builds; each `.c` file is otherwise self-contained and independently
compilable.

## Structure

- `code/` — numbered topic folders (`01_Loops`, `02_Functions`, `03_pointers`,
  `04_array`, `05_BitManipulation`, `06_Strings`, ...) each holding
  small example programs for that topic, plus a top-level `HelloWorld.c`. Folders
  `39`-`43` and `63`-`64` are a later batch covering senior/interview-level systems
  topics:
  `40_MemoryAlignmentAndPadding`, `41_FunctionPointersAndCallbacks`,
  `48_VolatileVsAtomicEmbedded`, `39_UndefinedBehaviorCatalog`,
  `73_LinkerAndLoaderMechanics`, `43_ErrnoAndErrorHandling`,
  `58_VirtualMemoryDeepDive`, `55_ContextSwitchMechanics`,
  `51_DeadlockDetectionAvoidance`, `72_FilesystemInternals`,
  `68_NetworkStackBasics`, `63_CacheCoherenceMESI`, `64_NUMABasics`. Several of these
  folders (and some added since) include a `NOTES.md` alongside their code: a concise,
  bullet-point, interview-focused writeup of the concept, distinct from a runnable demo -
  used when the topic is more conceptual than code (e.g. `68_NetworkStackBasics`,
  `63_CacheCoherenceMESI`, `64_NUMABasics` are NOTES.md-only, with no runnable demo,
  because a meaningful demo either duplicates an existing folder's code or needs
  hardware/environment this repo can't assume, e.g. multi-socket NUMA hardware).
  `79`-`83` are a later batch covering debugging/build tooling rather than language
  features: `79_GdbWorkflow` (buggy demo + gdb session transcript in NOTES.md),
  `80_ValgrindAndAsan` (leak/use-after-free/overflow demos, Valgrind vs ASan),
  `81_PerfAndStrace` (strace/ltrace/perf/`/usr/bin/time` notes + a cache-friendly
  vs cache-unfriendly loop demo), `82_StaticAndSharedLibraries` (one library built
  both as `.a` and `.so`, with `ldd`/`nm`/`objdump -T` notes), and `83_GenericMacro`
  (NOTES.md-only deep dive on `_Generic` selection mechanics, since `28_Generics`
  already has the runnable `_Generic` demo). A `84_PragmaPacking` folder was
  deliberately not created - `40_MemoryAlignmentAndPadding` already demonstrates
  `#pragma pack` directly, so a dedicated folder would just duplicate it.
- `Notes/` — numbered `.c` files that double as written notes/explanations (e.g.
  `04_MacroPreprocessor.c`, `06_Error_Signals.c`, `09_VariableScope.c`,
  `11_complicatedDeclaration.c`), plus two "Crack the Interview" PDF references.

## Working with this codebase

- `code/Makefile` builds every `.c`/`.cpp` file under `code/` into a matching binary under
  `code/bin/`, mirroring the source's directory structure (e.g.
  `code/05_BitManipulation/04_get_bitset.c` → `code/bin/05_BitManipulation/04_get_bitset`).
  From `code/`:
  ```
  make          # build all binaries into bin/
  make clean    # remove bin/
  ```
  `Notes/` is not covered by the Makefile; compile those files directly with gcc if needed,
  e.g. `gcc Notes/09_VariableScope.c -o /tmp/a.out && /tmp/a.out`.
- `code/05_BitManipulation/utils.h` is a small shared header (hex/decimal string checks,
  `linedisplay()`) used by files in that folder only — other topic folders do not share
  headers and each file is otherwise fully self-contained.
- New example files should follow the existing naming pattern within their folder:
  a two-digit numeric prefix followed by a short descriptive name
  (e.g. `03_OddOrEven.c`, `01_indexremoveproduct.c`).

