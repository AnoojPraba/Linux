# C_Basics/code

Index of the 82 numbered C topic folders (`01_Loops` to `82_CoroutinesInC`). Each folder has its own CLAUDE.md with files, build line, concepts, gotchas and cross-references.

## Build everything
- `make` here builds every `.c` (and the one `.cpp` in `12_Sorting`, plus `64_ExternC/01_main.cpp`) into `../bin/<NN_Topic>/<name>` (that is `C_Basics/bin/`, git-ignored), in parallel (`MAKEFLAGS += -j$(nproc)`). `make clean` removes `../bin`.
- Flags: `gcc -Wall -I. -pthread ... -lm` (no `-std` or `-O`, so gnu default and -O0).
- Special rules: `30_MultiFile` (`01_main.c` + `mathutils.c`), `54_OpaquePointer` (`01_main.c` + `handle.c`), `64_ExternC` (C object linked by g++), `71_CMakeIntroduction` (`01_hello.c` + `02_helper.c`; also has its own CMakeLists.txt). `mathutils.c` and `handle.c` have no `main()` and are excluded from the generic one-file rule.
- Single file by hand: `gcc -Wall -Wextra -std=gnu11 -pthread NN_Topic/file.c -o /tmp/x && /tmp/x` (add `-lm` for `52_TimeAndMath/02_mathFunctions.c`, `-I.` for `05_BitManipulation/04_get_bitset.c`).

## Numbering and layout
- Two-digit prefix gives the study order; there are no gaps from 01 to 82. Files inside a folder also carry `NN_` prefixes (a few early folders use plain names such as `forloop.c`, `Pattern.c`).
- NOTES-only folders (nothing to compile): `40_BTreeAndBPlusTree`, `72_CrossCompilationBasics`, `73_GenericMacro`.
- `common/utils.h` is a shared helper header (hex/decimal string checks), not a topic; only `05_BitManipulation/04_get_bitset.c` includes it. `HelloWorld.c` is a stray top-level example. `tags`, `cscope.*`, `compile_commands.json` are navigation/tooling artifacts.
- Folders `74`-`82` and `34_HashTable` end NOTES.md with a "Senior interviewer Q&A" section.
- Several demos are deliberately broken (UB, leaks, races, crashes): `57`, `62`, `65`, `66`, `67`, `68`, `70`. Do not fix them.
- Machine: results involving timing or addresses are specific to this aarch64 Raspberry Pi.

## Folder map
| Folder | Purpose |
|---|---|
| `01_Loops` | Warm-up loop exercises: for/while basics, digit reversal and Fibonacci |
| `02_Functions` | Short C trivia programs on float/double literals, macros, integer literal limits and variable scope |
| `03_pointers` | Pointer fundamentals through eight small programs: address/dereference, arithmetic, double pointers, function pointers,... |
| `04_array` | Array exercises: index tricks, copying, 2D arrays, passing arrays to functions |
| `05_BitManipulation` | Bit manipulation interview toolkit: single-bit idioms, XOR tricks, popcount, power-of-two, bit reversal, rotation and... |
| `06_EndiannessAndByteOrder` | Runtime endianness detection and byte swapping (manual vs htonl vs __builtin_bswap32) |
| `07_Strings` | C string basics: null termination, strlen/strcat/strcpy, hand-written string functions and strtok |
| `08_Structures` | Structs, nested structs and unions: assignment semantics, composition and shared storage |
| `09_Enum` | Enumerations in C: implicit numbering and using an enum to drive a state machine |
| `10_Search_alg` | Searching: linear, iterative binary, ternary search and the second-largest-element problem |
| `11_StringPatternMatching` | Substring search algorithms: naive, KMP and Rabin-Karp |
| `12_Sorting` | Sorting algorithms from O(n^2) basics to quick/merge/heap, counting and radix sort |
| `13_pattern` | Console pattern-printing exercises (stars/numbers, pyramids and diamonds) built from nested loops |
| `14_Recursion` | Recursion fundamentals: factorial, Fibonacci (naive exponential) and recursive binary search, contrasted with iterative... |
| `15_DynamicMemory` | Heap allocation basics: malloc vs calloc, realloc growth and freeing nested allocations |
| `16_ConstVolatile` | const and volatile: pointer constness cheat sheet and the embedded memory-mapped register idiom |
| `17_StorageClasses` | Storage duration and linkage: static locals, file-scope static, auto and register |
| `18_CommandLineArgs` | argc/argv basics |
| `19_Variadic` | Variadic functions with stdarg.h: a mini printf and a sum function |
| `20_ControlFlowExtras` | Less common control flow: goto for cleanup/nested-loop exit, assert and setjmp/longjmp |
| `21_QsortBsearch` | Using the C standard library qsort and bsearch with comparator callbacks |
| `22_AdvancedArrays` | C99 variable-length arrays and flexible array members |
| `23_RestrictQualifier` | The C99 restrict qualifier: a no-aliasing promise that lets the compiler optimise memory operations |
| `24_WideChars` | wchar_t and wide-string basics, including locale setup for wide output |
| `25_InlineFunctions` | inline functions vs macros and the (static) inline hint |
| `26_CommandLineOptions` | Option parsing with POSIX getopt |
| `27_BitFields` | Struct bit-fields: packing several small fields into a word, mirroring hardware registers or flags |
| `28_FileIO` | File I/O two ways: buffered stdio (text and binary) and raw POSIX system calls |
| `29_CompilerPipelineWalkthrough` | Hands-on walkthrough of the four compilation stages (preprocess, compile, assemble, link) run by hand on a trivial... |
| `30_MultiFile` | Splitting a program over several translation units: header, include guard |
| `31_Generics` | C11 _Generic: compile-time type dispatch for type-generic macros |
| `32_StackAndQueue` | Stacks, queues and deques (array and linked-list backed) plus the classic interview problems valid parentheses,... |
| `33_LinkedList` | Linked list family: singly, doubly and circular lists with insertion/deletion |
| `34_HashTable` | Hash tables from scratch: separate chaining and open addressing with tombstones |
| `35_ProbabilisticDataStructures` | Space-efficient approximate structures: Bloom filter and Count-Min Sketch implemented |
| `36_AdvancedHashingAndCacheAwareStructures` | Robin Hood open-addressing hashing implemented with probe-distance tracking |
| `37_LRUCache` | O(1) LRU cache combining a doubly-linked list (recency order) with a hash map (key to node) |
| `38_BinaryTree` | Binary search trees and the classic tree interview problems: insert/search, traversals, level order, deletion, LCA,... |
| `39_SelfBalancingTrees` | AVL and Red-Black trees: insertion, rotations and full deletion (including the double-black fixup) |
| `40_BTreeAndBPlusTree` | Conceptual notes on B-trees and B+ trees: why databases and filesystems use them (no code) |
| `41_Heap` | Binary heaps as arrays: min-heap, max-heap and heap sort |
| `42_UnionFind` | Disjoint-set union: naive version vs union-by-rank with path compression |
| `43_Graph` | Graph representations and core algorithms: BFS, DFS, Dijkstra, topological sort, Kruskal/Prim MST, Bellman-Ford and... |
| `44_Trie` | Prefix tree: insert/search, prefix queries (autocomplete) and recursive deletion with node pruning |
| `45_SegmentTreeAndFenwickTree` | Range-query structures: a range-sum segment tree and a Fenwick (binary indexed) tree, both O(log n) per query/update |
| `46_DynamicProgramming` | Dynamic programming staples: memoisation vs tabulation, 0/1 knapsack, LCS, LIS, coin change, edit distance and Kadane |
| `47_Backtracking` | Backtracking (choose, explore, un-choose): N-Queens, Sudoku, permutations and subset sum |
| `48_GreedyAlgorithms` | Greedy algorithms: activity selection, fractional knapsack |
| `49_SlidingWindowAndTwoPointer` | Two-pointer and sliding-window techniques for linear-time array/string problems |
| `50_ClassicArrayAndStringProblems` | Thirteen frequently asked array and string interview problems with idiomatic C solutions |
| `51_SkipList` | Skip list: a probabilistic ordered structure with expected O(log n) search/insert/delete and no rebalancing |
| `52_TimeAndMath` | time.h basics (epoch, localtime/struct tm) and libm math functions (radians, pow/sqrt, NaN handling) |
| `53_FixedPointArithmetic` | Q16.16 fixed-point arithmetic for FPU-less embedded targets |
| `54_OpaquePointer` | Opaque pointer (incomplete type) pattern for information hiding in C, the C analogue of C++ PIMPL |
| `55_VTableEmulation` | Emulating C++ virtual dispatch in C with a hand-rolled vtable (struct of function pointers) |
| `56_UnitTesting` | Minimal assert-style unit testing in C: test macros, counters and a CI-friendly exit code |
| `57_UndefinedBehaviorCatalog` | Catalog of the undefined behaviours an interviewer expects you to name |
| `58_ItoaAtoiSafeParsing` | Safe atoi with pre-checked overflow and a fast arbitrary-base itoa, including the INT_MIN trap |
| `59_MemoryAlignmentAndPadding` | Natural alignment, struct padding (member ordering changes sizeof) and alignas |
| `60_AlignedMallocFree` | Implementing aligned malloc/free by hand with the bitmask round-up trick and a stashed original pointer |
| `61_FunctionPointersAndCallbacks` | Function-pointer idioms: dispatch tables and callback registration (observer-style event source) |
| `62_StrictAliasing` | Demonstrates a strict-aliasing violation (int* and float* to the same memory) and the memcpy fix |
| `63_MemmoveImplementation` | Hand-written memmove that handles overlapping regions by copying forward or backward |
| `64_ExternC` | Calling C code from C++ with extern "C" (name mangling and linkage) |
| `65_SecurityDemos` | Three classic memory-safety vulnerability patterns with the fix: stack buffer overflow and canary, format string |
| `66_ThreadSanitizerDemo` | A deliberately racy shared counter that ThreadSanitizer reliably detects |
| `67_GdbWorkflow` | GDB workflow against a deliberately crashing program: breakpoints, watchpoints, backtraces, memory/registers, core... |
| `68_ValgrindAndAsan` | Memory-error detection with valgrind Memcheck and AddressSanitizer using three deliberately broken programs (leak,... |
| `69_PerfAndStrace` | Profiling and tracing tools (perf, strace, ltrace, /usr/bin/time) via a cache-friendly vs cache-hostile loop demo |
| `70_CombinedDebuggingCaseStudy` | Case study tying gdb, ASan and TSan together on one program with two independent seeded bugs |
| `71_CMakeIntroduction` | Introduction to CMake using a two-file C program and a minimal CMakeLists.txt |
| `72_CrossCompilationBasics` | Conceptual notes on cross-compilation: toolchain triplets, sysroot, CMake toolchain files and QEMU emulation (no code) |
| `73_GenericMacro` | Conceptual notes on C11 _Generic type-generic macros compared with void*-based generic C (see 31_Generics for the code) |
| `74_ContainerOfAndIntrusiveLists` | Linux-kernel style container_of/offsetof, intrusive lists and flexible array members |
| `75_IntegerPromotionsAndConversions` | Usual arithmetic conversions, integer promotion and signed/unsigned traps |
| `76_ErrorHandlingAndCleanupPatterns` | C error-handling and resource-cleanup idioms: goto cleanup, GCC cleanup attribute and setjmp/longjmp |
| `77_PreprocessorAndC11Tricks` | Senior-level preprocessor idioms (X-macros, stringify/paste, variadic macros) and C99/C11 features (static_assert,... |
| `78_BranchHintsPrefetchAndCacheLayout` | Microarchitecture-aware C: branch prediction and likely/unlikely hints, array-of-structures vs structure-of-arrays |
| `79_StackFramesAndCallingConvention` | Stack layout, frame pointers and the x86-64 SysV vs AArch64 (AAPCS64) calling conventions, observed with a... |
| `80_SignalSafetyAndThreadLocal` | Async-signal-safe handler rules and thread-local storage (_Thread_local) with pthread_once |
| `81_MallocInternalsAndAllocators` | How malloc works: a free-list allocator with split/coalesce, a size-class (slab) allocator |
| `82_CoroutinesInC` | Coroutines in C: stackful with ucontext and stackless with the switch/__LINE__ (protothreads) trick |

## Cross-domain links
- OS internals (threads, signals, sockets, allocators) live in `../../OS/code`; C++ equivalents in `../../Cpp/code`; system-design context in `../../SystemDesign/topics`.
- Drill list: `../Notes/TIMED_DRILLS.md` (write-from-memory drills mapped to these folders).
