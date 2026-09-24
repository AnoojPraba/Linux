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
  `04_array`, `05_BitManipulation`, `06_EndiannessAndByteOrder`, `07_Strings`, ...)
  each holding small example programs for that topic, plus a top-level
  `HelloWorld.c`. `06_EndiannessAndByteOrder` covers runtime endianness detection,
  manual byte-swapping vs `htonl`/`__builtin_bswap32`, and cross-references the
  `htons`/`htonl` calls in `../OS/code`'s socket folders.
  `11_StringPatternMatching` (inserted right after `10_Search_alg`, before
  `12_Sorting`) covers naive/KMP/Rabin-Karp substring search. `12_Sorting`
  (originally quicksort-less) was expanded with quicksort, mergesort, and a
  fresh heapsort implementation alongside the original `Simplesort.cpp`, plus
  a NOTES.md on complexity/stability/in-place tradeoffs.
  `29_CompilerPipelineWalkthrough` (inserted right after `28_FileIO`, before
  `30_MultiFile`) is a NOTES.md-driven walkthrough of preprocessing/compilation/
  assembly/linking (`gcc -E`/`-S`/`-c`/link) run by hand against a trivial
  companion demo. `32`-`46` are a data-structures batch (inserted right after
  `31_Generics`, before the old `39_TimeAndMath`, shifting everything from
  there onward up by 8 in total): `32_StackAndQueue` (array/linked-list stacks
  and queues, plus a circular array-based deque), `33_LinkedList`,
  `34_HashTable` (separate chaining reusing the linked-list node pattern, and
  open addressing with tombstone-based deletion), `35_ProbabilisticDataStructures`
  (Bloom filter and Count-Min Sketch in code; HyperLogLog covered
  conceptually in NOTES.md only), `36_AdvancedHashingAndCacheAwareStructures`
  (Robin Hood open-addressing hashing in code, with probe-distance tracking
  to demonstrate its variance-equalizing effect; cache-oblivious algorithms
  and Judy arrays covered conceptually in NOTES.md only), `37_LRUCache`
  (inserted right after `36_AdvancedHashingAndCacheAwareStructures`, before
  `38_BinaryTree`, shifting everything from there through the old
  `62_GenericMacro` up by 1 - the O(1) LRU cache combining a doubly-linked
  list with a hash map, since this repo already has both building blocks),
  `38_BinaryTree`, `39_SelfBalancingTrees` (AVL insertion/rotations and full
  Red-Black insertion/lookup in code, since a black-height invariant gives
  red-black trees the same O(log n) worst-case lookup guarantee as AVL;
  Red-Black deletion is intentionally left conceptual-only, since it is
  notoriously intricate), `40_BTreeAndBPlusTree` (NOTES.md-only -
  cross-references `../OS/code/54_FileSystemStructuresAndAllocation` and
  `../SystemDesign/topics/04_DatabaseIndexingAndQueryOptimization`),
  `41_Heap`, `42_UnionFind` (naive and union-by-rank/path-compression
  variants), `43_Graph`, `44_Trie`, `45_SegmentTreeAndFenwickTree`,
  `46_DynamicProgramming`, and `47_SkipList`.
  Folders `52`-`57` are a later batch covering senior/interview-level
  language-level systems topics: `52_UndefinedBehaviorCatalog`,
  `53_ItoaAtoiSafeParsing` (inserted right after `52_UndefinedBehaviorCatalog`,
  before `54_MemoryAlignmentAndPadding` - safe bounded `atoi` and an
  arbitrary-base `itoa`, since both hinge on the signed-overflow-is-UB
  background the preceding folder covers), `54_MemoryAlignmentAndPadding`,
  `55_AlignedMallocFree` (inserted right after `54_MemoryAlignmentAndPadding`,
  before `56_FunctionPointersAndCallbacks` - a hand-rolled `aligned_malloc`/
  `aligned_free` using the bitmask alignment trick, cross-referencing
  `../OS/code/38_FalseSharing` for the cache-line-alignment motivation),
  `56_FunctionPointersAndCallbacks`, `57_StrictAliasing`.
  `58`-`67` are a later batch covering debugging/build tooling and remaining
  core-C topics: `58_MemmoveImplementation` (inserted right after
  `57_StrictAliasing`, before `59_ExternC` - a hand-rolled `memmove` that
  detects overlap direction, unlike `memcpy`), `59_ExternC` (a C++ caller
  linking against a C-compiled TU), `60_SecurityDemos`,
  `61_ThreadSanitizerDemo`, `62_GdbWorkflow` (buggy demo + gdb session
  transcript in NOTES.md), `63_ValgrindAndAsan` (leak/use-after-free/
  overflow demos, Valgrind vs ASan, plus a Helgrind-vs-ThreadSanitizer
  NOTES.md section), `64_PerfAndStrace` (strace/ltrace/perf/`/usr/bin/time`
  notes + a cache-friendly vs cache-unfriendly loop demo, plus a gprof
  NOTES.md section with real flat-profile output),
  `65_CombinedDebuggingCaseStudy` (inserted right after `64_PerfAndStrace`,
  before `66_CMakeIntroduction` - one small demo seeding both a heap
  buffer overflow and a data race, with a NOTES.md walkthrough tying
  together GDB/ASan/TSan/perf and matching the tool to the bug class), and
  `66_CMakeIntroduction` (a minimal two-file CMake project - `CMakeLists.txt`
  building `01_hello.c`/`02_helper.c` - alongside NOTES.md on CMake
  vocabulary and why projects use it over hand-written Makefiles).
  `67_GenericMacro` (NOTES.md-only deep dive on `_Generic` selection mechanics,
  since `31_Generics` already has the runnable `_Generic` demo). A dedicated
  `PragmaPacking` folder was deliberately not created - `54_MemoryAlignmentAndPadding`
  already demonstrates `#pragma pack` directly, so a dedicated folder would just
  duplicate it.
  OS-internals topics (processes, threads, synchronization, scheduling, memory
  management, IPC/RPC, filesystems, networking, boot process, dynamic linking,
  static/shared libraries, etc.) have been split out into a separate sibling
  repo, `../OS/` (see `../OS/CLAUDE.md` and `../OS/code/`), since this repo is
  meant for core C-language/tooling topics only.
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

