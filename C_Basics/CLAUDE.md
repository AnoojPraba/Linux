# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with
code in this repository.

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
  now also has AVL deletion (`03_avlTreeDeletion.c`) and full Red-Black
  deletion with the double-black fixup (`04_redBlackTreeDeletion.c`)),
  `40_BTreeAndBPlusTree` (NOTES.md-only -
  cross-references `../OS/code/57_FileSystemStructuresAndAllocation` and
  `../SystemDesign/topics/04_DatabaseIndexingAndQueryOptimization`),
  `41_Heap`, `42_UnionFind` (naive and union-by-rank/path-compression
  variants), `43_Graph`, `44_Trie`, `45_SegmentTreeAndFenwickTree`,
  `46_DynamicProgramming`, and `47_Backtracking`/`48_GreedyAlgorithms`/
  `49_SlidingWindowAndTwoPointer` (inserted right after `46_DynamicProgramming`,
  before the old `47_SkipList`, shifting everything from there through the old
  `67_GenericMacro` up by 3 in total): `47_Backtracking` (N-Queens, Sudoku
  solver, permutations, subset-sum), `48_GreedyAlgorithms` (activity selection,
  fractional knapsack, and a coin-change greedy demo that cross-references
  `46_DynamicProgramming/02_knapsack.c` and `05_coinChangeMinCoins.c` to
  contrast the greedy approach with the DP one), and
  `49_SlidingWindowAndTwoPointer` (two-pointer pair-sum, fixed-window max sum,
  longest-unique-substring). `50_SkipList`.
  Folders `56`-`61` are a later batch covering senior/interview-level
  language-level systems topics: `56_UndefinedBehaviorCatalog`,
  `57_ItoaAtoiSafeParsing` (inserted right after `56_UndefinedBehaviorCatalog`,
  before `58_MemoryAlignmentAndPadding` - safe bounded `atoi` and an
  arbitrary-base `itoa`, since both hinge on the signed-overflow-is-UB
  background the preceding folder covers), `58_MemoryAlignmentAndPadding`,
  `59_AlignedMallocFree` (inserted right after `58_MemoryAlignmentAndPadding`,
  before `60_FunctionPointersAndCallbacks` - a hand-rolled `aligned_malloc`/
  `aligned_free` using the bitmask alignment trick, cross-referencing
  `../OS/code/39_FalseSharing` for the cache-line-alignment motivation),
  `60_FunctionPointersAndCallbacks`, `61_StrictAliasing`.
  `53_FixedPointArithmetic` (inserted right after `52_TimeAndMath`, before the
  old `53_OpaquePointer` which shifted to `54_OpaquePointer` along with
  everything through the old `71_GenericMacro`) covers Q16.16 fixed-point
  conversion/add/sub/mul/div (with the multiply/divide rescaling gotcha) and
  a fixed-point-vs-float comparison (determinism/no-FPU-required tradeoffs
  vs floating point's dynamic range).
  `62`-`71` are a later batch covering debugging/build tooling and remaining
  core-C topics: `62_MemmoveImplementation` (inserted right after
  `61_StrictAliasing`, before `63_ExternC` - a hand-rolled `memmove` that
  detects overlap direction, unlike `memcpy`), `63_ExternC` (a C++ caller
  linking against a C-compiled TU), `64_SecurityDemos`,
  `65_ThreadSanitizerDemo`, `66_GdbWorkflow` (buggy demo + gdb session
  transcript in NOTES.md), `67_ValgrindAndAsan` (leak/use-after-free/
  overflow demos, Valgrind vs ASan, plus a Helgrind-vs-ThreadSanitizer
  NOTES.md section), `68_PerfAndStrace` (strace/ltrace/perf/`/usr/bin/time`
  notes + a cache-friendly vs cache-unfriendly loop demo, plus a gprof
  NOTES.md section with real flat-profile output),
  `69_CombinedDebuggingCaseStudy` (inserted right after `68_PerfAndStrace`,
  before `71_CMakeIntroduction` - one small demo seeding both a heap
  buffer overflow and a data race, with a NOTES.md walkthrough tying
  together GDB/ASan/TSan/perf and matching the tool to the bug class), and
  `71_CMakeIntroduction` (a minimal two-file CMake project - `CMakeLists.txt`
  building `01_hello.c`/`02_helper.c` - alongside NOTES.md on CMake
  vocabulary and why projects use it over hand-written Makefiles).
  `72_CrossCompilationBasics` (inserted right after `71_CMakeIntroduction`,
  before the old `71_GenericMacro` which shifted to `73_GenericMacro`) is a
  NOTES.md-only topic covering cross-compilation vs this repo's own native
  builds (x86_64 desktop and Raspberry Pi ARM, each with its own native gcc),
  toolchain triplets, sysroots, CMake's `CMAKE_TOOLCHAIN_FILE`, and QEMU
  emulation for testing cross-compiled binaries.
  `73_GenericMacro` (NOTES.md-only deep dive on `_Generic` selection mechanics,
  since `31_Generics` already has the runnable `_Generic` demo). A dedicated
  `PragmaPacking` folder was deliberately not created - `58_MemoryAlignmentAndPadding`
  already demonstrates `#pragma pack` directly, so a dedicated folder would just
  duplicate it.
  OS-internals topics (processes, threads, synchronization, scheduling, memory
  management, IPC/RPC, filesystems, networking, boot process, dynamic linking,
  static/shared libraries, etc.) have been split out into a separate sibling
  repo, `../OS/` (see `../OS/CLAUDE.md` and `../OS/code/`), since this repo is
  meant for core C-language/tooling topics only.
  A later batch (bringing the total, before the `53_FixedPointArithmetic`/
  `72_CrossCompilationBasics` additions, to `01_Loops` through
  `71_GenericMacro`) added classic interview staples that were previously
  missing: deletion operations for
  `33_LinkedList`/`38_BinaryTree`/`39_SelfBalancingTrees`/`44_Trie` (which were
  insert-only before), missing sorts in `12_Sorting` (bubble/selection/insertion/
  counting/radix), circular linked lists, a standalone max-heap in `41_Heap`,
  classic graph algorithms in `43_Graph` (topological sort, Kruskal's/Prim's MST,
  Bellman-Ford, Floyd-Warshall), more bitwise tricks in `05_BitManipulation`
  (set-bit counting, power-of-2 check, XOR swap/single-number/missing-number,
  bit set/clear/toggle/check, bit reversal/rotation), more stack/tree/list
  problems (valid parentheses, min-stack, LCA, tree diameter, invert/balanced/
  symmetric checks, merge-sorted-lists, find-middle), and a new
  `50_ClassicArrayAndStringProblems` folder (inserted right after
  `49_SlidingWindowAndTwoPointer`, before the old `50_SkipList` which shifted to
  `51_SkipList` along with everything after it) covering two-sum, palindrome/
  anagram checks, merge-intervals, rotate-array, spiral-matrix, best-time-to-buy-
  sell-stock, climbing-stairs, trapping-rain-water, and container-with-most-water.
  The final layout runs contiguously `01_Loops` through `73_GenericMacro`.
  `74`-`80` are a later senior-level C batch: `74_ContainerOfAndIntrusiveLists`
  (`offsetof`/`container_of`, kernel-style intrusive list, flexible array
  members), `75_IntegerPromotionsAndConversions` (promotion rules, usual
  arithmetic conversions, signed/unsigned traps, sequence points),
  `76_ErrorHandlingAndCleanupPatterns` (`goto cleanup` ladder,
  `__attribute__((cleanup))`, `setjmp`/`longjmp`), `77_PreprocessorAndC11Tricks`
  (X-macros, stringify/paste, double-evaluation, `_Static_assert`, designated
  initializers, compound literals), `78_BranchHintsPrefetchAndCacheLayout`
  (`__builtin_expect`, branch-misprediction timing, AoS vs SoA, software
  prefetch), `79_StackFramesAndCallingConvention` (SysV x86-64 vs AAPCS64,
  frame inspection, stack-smashing mitigations), and
  `80_SignalSafetyAndThreadLocal` (async-signal-safety, self-pipe,
  `_Thread_local`, `pthread_once`). The matching OS-level topic for lock
  internals is `../OS/code/65_FutexAndSeqlock`.
  `81_MallocInternalsAndAllocators` (a first-fit free-list `malloc` with split/coalesce,
  a size-class slab allocator, and a brk-vs-mmap/RSS observation of glibc; NOTES.md covers
  ptmalloc arenas/bins/tcache, jemalloc/tcmalloc and hardening) and
  `82_CoroutinesInC` (stackful `ucontext` coroutines and stackless switch-based
  protothreads). Folders `74`-`82` end their NOTES.md with a "Senior interviewer Q&A"
  section; `34_HashTable/NOTES.md` was backfilled the same way.
  `Notes/TIMED_DRILLS.md` is a "write it from memory in N minutes" drill list (40 coding
  drills mapped to the reference folders here and in `../OS`, plus debug and explain drills).
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

