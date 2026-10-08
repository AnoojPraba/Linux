# Timed "Write It From Memory" Drills

Reading a reference implementation is not the same as producing one under pressure.
For each drill: set the timer, write it on paper/plain editor (no autocomplete, no
copy), compile with `gcc -Wall -Wextra -fsanitize=address,undefined`, test the edge
cases, THEN compare with the reference folder. Re-do any you fail after a few days
(spaced repetition). Paths are relative to `C_Basics/code/` unless noted.

Interviewers watch for: clarifying questions, edge cases (empty, one element, NULL,
overflow, duplicates), complexity statement, memory ownership/leaks, `const`
correctness, naming, testing instinct, and calm debugging.

## Warm-ups (5-10 min)
| # | Task | Edge cases to cover | Reference |
|---|---|---|---|
| 1 | Reverse a string in place; reverse words in a sentence | empty, one char, trailing spaces | `07_Strings` |
| 2 | `strlen`, `strcpy`, `strcmp`, `strncpy` (and say why `strncpy` is a trap) | NULL, overlap, no terminator | `07_Strings` |
| 3 | `atoi` with overflow detection; `itoa` any base | `INT_MIN`, sign, whitespace | `58_ItoaAtoiSafeParsing` |
| 4 | Count set bits (3 ways), power-of-two check, swap w/o temp, find the single non-repeated number | 0, `INT_MIN`, negative | `05_BitManipulation` |
| 5 | Detect endianness; byte-swap 32-bit | | `06_EndiannessAndByteOrder` |
| 6 | `memcpy` and `memmove` (explain overlap) | n=0, overlap forward/back | `63_MemmoveImplementation` |
| 7 | Binary search (iterative) + first/last occurrence | `mid` overflow, duplicates, empty | `10_Search_alg` |
| 8 | Fibonacci / factorial iterative vs recursive; stack-depth discussion | n=0, overflow | `14_Recursion` |

## Core data structures (15-30 min)
| # | Task | Must state | Reference |
|---|---|---|---|
| 9 | Singly linked list: insert/delete/reverse/find middle/detect cycle (Floyd) | O(1) head ops, pointer-to-pointer delete | `33_LinkedList` |
| 10 | Merge two sorted lists; remove nth from end | dummy head trick | `33_LinkedList` |
| 11 | Stack + queue (array and list), circular buffer, min-stack | full/empty, wraparound | `32_StackAndQueue` |
| 12 | Hash table (chaining) with resize | load factor, hash fn, ownership | `34_HashTable` |
| 13 | LRU cache O(1) get/put | DLL + hash, update on get | `37_LRUCache` |
| 14 | BST insert/search/delete/inorder; validate BST; LCA | delete 2 children, recursion depth | `38_BinaryTree` |
| 15 | Min-heap / priority queue; heapsort | sift up/down, 0-based indices | `41_Heap`, `12_Sorting` |
| 16 | Trie insert/search/prefix | child array vs map, free | `44_Trie` |
| 17 | Graph BFS/DFS, cycle detection, topological sort | visited, disconnected | `43_Graph` |
| 18 | Union-Find with path compression + rank | amortized bound | `42_UnionFind` |
| 19 | Quicksort (partition), mergesort, explain stability/complexity | worst case, pivot choice, in-place | `12_Sorting` |
| 20 | Two-sum, sliding window max/longest unique substring, merge intervals | duplicates, empty | `49_SlidingWindowAndTwoPointer`, `50_ClassicArrayAndStringProblems` |

## Systems-level C (20-45 min) - what senior loops ask
| # | Task | Must discuss | Reference |
|---|---|---|---|
| 21 | Implement `malloc/free` (free list, split, coalesce, alignment) | fragmentation, thread safety, `sbrk` vs `mmap` | `81_MallocInternalsAndAllocators` |
| 22 | Arena/bump allocator and fixed-size pool | O(1), alignment, reset | `../../OS/code/45_CustomAllocator` |
| 23 | `aligned_malloc/free` | stash raw pointer, power-of-two mask | `60_AlignedMallocFree` |
| 24 | Ring buffer: SPSC lock-free with correct atomics | full vs empty, acquire/release, false sharing | `../../OS/code/37_LockFreeRingBuffer` |
| 25 | Thread-safe bounded queue (mutex + 2 condvars), thread pool | spurious wakeups, shutdown | `../../OS/code/73_ConcurrentTcpServers` |
| 26 | Producer/consumer with semaphores; dining philosophers (deadlock-free) | lock ordering | `../../OS/code/13_ClassicalSyncAlgorithms`, `14_SyncProblems` |
| 27 | Reader-writer lock (fair) | writer starvation | `../../OS/code/21_AdvancedSyncPrimitives` |
| 28 | Spinlock with atomics; mutex on futex | TTAS, backoff, 3 states | `../../OS/code/65_FutexAndSeqlock` |
| 29 | Treiber stack and explain ABA | tag/hazard pointers | `../../OS/code/66_MemoryModelLitmusTests` |
| 30 | `container_of` + intrusive list | offsetof, multiple lists | `74_ContainerOfAndIntrusiveLists` |
| 31 | Generic container with `void *` or `_Generic`; function-pointer callbacks (qsort-style) | type safety, ownership | `31_Generics`, `61_FunctionPointersAndCallbacks`, `21_QsortBsearch` |
| 32 | Simple tokenizer/parser (INI, CSV, key=value, arithmetic expression with precedence) | error handling, buffer bounds | `Notes/`, `27_BitFields` for formats |
| 33 | TCP echo server (iterative -> epoll); read exactly N bytes; length-prefixed framing | partial reads, EINTR, EAGAIN | `../../OS/code/52_SocketProgramming`, `67_EpollInDepth`, `69_TcpDeepDive` |
| 34 | UART frame parser (SOF/len/CRC) state machine | resync, ring buffer from ISR | `../../OS/code/75_UartSerialProgramming` |
| 35 | I2C: write the master read-register sequence; explain ACK/NACK | repeated START | `../../OS/code/76_I2cBusProtocol` |
| 36 | Signal-safe handler with flag + self-pipe | async-signal-safety, EINTR | `80_SignalSafetyAndThreadLocal` |
| 37 | `fork/exec/wait` mini shell with pipes and redirection | zombies, closing fds, `dup2` | `../../OS/code/02_Processes`, `48_IPC` |
| 38 | `mmap` a file and count lines; shared memory between processes | alignment, `msync`, SIGBUS | `../../OS/code/35_MmapFile` |
| 39 | LRU/LFU cache, Bloom filter, rate limiter (token bucket) | time source, concurrency | `37_LRUCache`, `35_ProbabilisticDataStructures` |
| 40 | Memory pool + object lifecycle with cleanup ladder (goto) | error paths, double free | `76_ErrorHandlingAndCleanupPatterns` |

## Debug drills (10-15 min each)
Take a provided buggy snippet, find the bug aloud, fix it, and say how a tool would
have caught it: use-after-free/overflow (`68_ValgrindAndAsan`), data race (`66_ThreadSanitizerDemo`),
signed overflow/strict aliasing/uninitialized read (`57_UndefinedBehaviorCatalog`, `62_StrictAliasing`),
`sizeof` on a pointer param, `char` signedness in `ctype`, `i = i++`, off-by-one in loops
with unsigned (`75_IntegerPromotionsAndConversions`), `realloc` leak, missing `volatile`
or atomic, deadlock lock-order (`../../OS/code/17_DeadlockDetectionAvoidance`).

## "Explain" drills (5 min spoken, whiteboard allowed)
What happens when you type `ls | wc -l`; what happens on `malloc(1<<30)`; page fault
walk-through (`../../OS/code/33_VirtualMemoryDeepDive`); how a mutex works
(`../../OS/code/65_FutexAndSeqlock`); how `printf` reaches the terminal; what `main`'s
caller does; what a context switch saves; TLB miss vs page fault; why `volatile` isn't
thread-safe; stack vs heap; how a shared library is found/loaded
(`../../OS/code/62_LinkerAndLoaderMechanics`); TCP handshake and teardown
(`../../OS/code/69_TcpDeepDive`).

## Schedule suggestion
Daily: 2 drills (one data structure, one systems), 10 min each of explain/debug
drills. Weekly: re-run failed drills, one 45-minute mock with a friend, review a past
solution for style (naming, `const`, error paths). Track pass/fail in a table; aim for
"correct, tested, and explained in <25 minutes" on all of 1-40.
