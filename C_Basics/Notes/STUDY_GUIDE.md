# C Study Guide

A learning path through this repo's `Notes/` and `code/` files, in the order they build on
each other. Each topic links to the file(s) that cover it and gives a short recap plus any
gaps those files don't already fill.

## 1. Getting started

- **What C is, program structure, compiling** — `01_introduction.c`
- **The `int` keyword, signed range/overflow** — `02_int_keyword.c`
- **`int main()` vs `int main(void)`** — `03_int_main_void.c`
- **Escape sequences** (`\n`, `\t`, `\0`, ...) — `07_escape_seq.c`
- **Tokens** (keywords, identifiers, constants, operators) — `08_Tokens.c`

Try it: `code/HelloWorld.c` is the minimal example of the structure described in
`01_introduction.c`.

## 2. Compilation pipeline

- **Preprocessing → Compilation → Assembly → Linking**, with the intermediate file
  extensions (`.i`, `.s`, `.o`) and how to keep them with `-save-temps` — `05_compilation.c`
- **The preprocessor itself**: `#include`, `#define`, macro arguments, `##` concatenation,
  `#ifdef`/`#ifndef`/`defined`, predefined macros (`__FILE__`, `__LINE__`, ...), `#undef`
  — `04_MacroPreprocessor.c`. See `code/02_Functions/04_define.c` for a working example.

## 3. Data types and operators

- Signed integer representation and overflow is covered in `02_int_keyword.c`; the
  `code/02_Functions/0*_datatype.c` files exercise the other basic types
  (`char`, `float`, `double`, `unsigned`, sizes via `sizeof`).
- **Operators**: unary, binary (arithmetic/bitwise), ternary — summarized in
  `08_Tokens.c` under "operators". Bitwise operators specifically are covered in depth in
  section 8 below.

## 4. Control flow and loops

Not covered by dedicated notes — build the mental model directly from `code/01_Loops/`:
- `forloop.c` — basic `for` loop mechanics.
- `SumFib.c` — loop-driven accumulation/recurrence (Fibonacci sum).
- `reverse.c` — using a loop to reverse a sequence (e.g. digits of a number).

Key rule to internalize: a `for`/`while` is just a condition checked before each
iteration; a `do...while` checks it after, so the body always runs at least once.

## 5. Functions

- `code/02_Functions/` has the runnable examples (datatypes, `05_int_max.c` for a
  function returning a computed max, `07_scope.c`/`06_scope.c` for scope demos).
- **Variable scope and lexical scoping** (file scope, block scope, function-prototype
  scope, function scope; why a variable shadowed in a caller doesn't leak into a callee)
  — `09_VariableScope.c`, demonstrated live in `code/02_Functions/06_scope.c` and
  `07_scope.c`.
- **Linking and symbol resolution**: strong vs. weak symbols, and the linker rules for
  picking one definition when a variable/function is defined in more than one translation
  unit — `10_LinkerResolvesMultipleDefinition.c`. Read `05_compilation.c` first — this
  topic only makes sense once you know linking is a separate stage from compiling.

## 6. Arrays

Not covered by dedicated notes — the `code/04_array/` examples form the progression:
1. `03_array_print.c` — declare, fill, and print a 1D array.
2. `04_average.c` — accumulate over an array (`sum`, then divide).
3. `02_array_copy.c` — copying an array both by hand (element loop) and with `memcpy`.
4. `01_indexremoveproduct.c` — a classic "product of array except self" problem, with an
   O(n²) and an O(n) approach side by side (read the comments — this is a good exercise
   for reasoning about algorithmic complexity, not just C syntax).
5. `2d_print.c`, `sum2array.c`, `parseArrayToFunction.c` — 2D arrays: declaration
   (`int temperature[CITY][WEEK]`), nested-loop traversal, and how a fixed-size 2D array
   is passed to a function (`void f(int num[2][2])` — note only the first dimension of a
   multi-dimensional array parameter may be omitted, the rest must be fixed at compile
   time).

## 7. Pointers

Not covered by dedicated notes — `code/03_pointers/` builds the concept in two steps:
1. `01_changeValueWithPointers.c` — `&` (address-of) and `*` (dereference); a pointer just
   holds an address, and dereferencing it reads/writes the value at that address.
2. `02_pointers.c` — same idea, but shows the pointer tracking a variable across
   reassignment (`pc` keeps pointing at `c` even after `c`'s value changes), and writing
   through the pointer (`*pc = 2`) to mutate the original variable.
3. `03_pointerArithmetic.c` — `p + i` advances by `sizeof(*p)` bytes, not 1; walking an
   array via pointer arithmetic instead of indexing, and subtracting two pointers into
   the same array to get an element count.
4. `04_doublePointer.c` — `int **` as "a pointer to a pointer"; the one case where a
   double pointer genuinely earns its keep is an out-parameter a function must reassign
   (e.g. handing back a freshly `malloc`'d buffer), per the double-pointer-avoidance rule
   in the project's coding conventions.
5. `05_functionPointer.c` — declaring/assigning a function pointer, passing one as a
   callback (`applyOp`), and building a small dispatch table with an array of function
   pointers.
6. `06_voidPointer.c` — a `void *` holds an address with no type information attached, so
   it must be cast to a concrete pointer type before dereferencing; this is how generic
   APIs (e.g. `qsort`, `memcpy`) accept data of any type.
7. `07_pointersAndArrays.c` — an array name decays to a pointer to its first element in
   most expressions (`arr == &arr[0]`), which is why `arr[i]` and `*(arr + i)` are
   equivalent; but `sizeof(arr)` only sees the whole array's size in the scope where it
   was declared — once passed as a function parameter, it has already decayed and
   `sizeof` reports the pointer's size instead (see the `-Wsizeof-array-argument` warning
   this file intentionally triggers).
8. `08_pointersAndStrings.c` — a `char arr[] = "..."` copies the string into writable
   memory, while a `char *str = "..."` points at a string literal that is typically
   read-only (writing through it is undefined behavior, though reassigning `str` itself
   is fine); also covers walking a string via pointer until `'\0'`, and an array of
   `char *` where each element points at a separate literal.

Then tackle the hardest pointer topic in the repo:

- **Reading complicated declarations** (pointers to functions, etc.) via postfix
  conversion (innermost parens → right → left) — `11_complicatedDeclaration.c`. Once this
  clicks, any declaration involving `*`, `[]`, and `()` together becomes mechanical to
  parse instead of memorized.

## 8. Bit manipulation

`code/05_BitManipulation/` is a self-contained mini-module (shares `utils.h` for
hex/decimal input parsing, see the project CLAUDE.md):
1. `02_Boolean.c` — using bitwise ops for boolean-style logic.
2. `03_OddOrEven.c` — checking a bit (`n & 1`) to classify a number.
3. `01_BitManipulation.c` — general set/clear/toggle/check-bit operations.
4. `04_get_bitset.c` — the most complete example: parses hex or decimal input, prints the
   full binary representation with bit-position ruler (`printBinary`), and prints
   compressed ranges of set bits (e.g. `[0-3 5 7-9]`) via `bitSet`.

## 9. Searching, sorting, and patterns

- `code/09_Search_alg/02_LinearSearch.c` — the baseline O(n) search: check every element,
  no ordering assumption. Start here before the others.
- `code/09_Search_alg/03_BinarySearchIterative.c` — O(log n) search on a *sorted* array,
  halving the search range each iteration; compare against the recursive version in
  section 11 (`12_Recursion/03_recursiveBinarySearch.c`) to see the same logic with and
  without a call stack.
- `code/09_Search_alg/01_Second_Largest.c` — single-pass second-largest-in-array by
  tracking two running values (`max`, `sec_max`) instead of sorting first; a good example
  of trading a full sort (O(n log n)) for a linear scan (O(n)) when you only need one
  statistic.
- `code/10_Sorting/Simplesort.cpp` — a basic sort implementation for comparison (note:
  this one is C++, not C — useful to see how similar the syntax is for simple algorithms).
- `code/11_pattern/Pattern.c` — nested-loop pattern printing (triangles, etc.); a good
  drill for nested-loop index reasoning, independent of any data structure.

## 10. Structs, unions, and strings

Not covered by dedicated notes — build the mental model directly from the examples:
- `code/07_Structures/01_basicStruct.c` — declaring a `struct`, a `typedef`'d struct,
  member access (`.` vs `->` through a pointer), and that struct assignment copies every
  member (independent copies, not aliasing).
- `code/07_Structures/02_nestedStruct.c` — a struct containing struct members
  (`Rectangle` made of two `Point`s), and an array of structs.
- `code/07_Structures/03_union.c` — a `union`'s members all share the same memory (unlike
  a struct, where each member has its own space), so `sizeof(union)` is just the size of
  its largest member.
- `code/06_Strings/01_stringBasics.c` — a C string is a `char` array terminated by `\0`;
  `strlen`/`strcpy`/`strcat`/`strcmp`/`strchr` from `<string.h>`.
- `code/06_Strings/02_manualStringFunctions.c` — hand-rolled `strlen`/`strcpy`/`strrev` to
  see what the library functions are doing under the hood.
- `code/06_Strings/03_tokenizeString.c` — `strtok` splits a string in place (it overwrites
  each delimiter with `\0`), which is why later calls pass `NULL` to continue the same
  tokenization.

## 11. Recursion

Not covered by dedicated notes — `code/12_Recursion/` builds the concept in three steps:
- `01_factorial.c` — the canonical base-case + recursive-case example, next to an
  iterative version for comparison.
- `02_fibonacci.c` — naive recursion's hidden cost (`fibNaive` is O(2^n) because it
  recomputes the same sub-values repeatedly), fixed by memoizing into a cache array
  (O(n)). This is the key intuition for why "recursive" doesn't automatically mean
  "slow" or "fast" — it depends on whether work is being repeated.
- `03_recursiveBinarySearch.c` — recursion applied to a real algorithm, each call
  shrinking the search range by half; compare against an iterative binary search if you
  write one, to see the same logic without a call stack.

## 12. Dynamic memory

Not covered by dedicated notes — `code/13_DynamicMemory/` extends the single
`malloc`/`free` pair already seen in `03_pointers/04_doublePointer.c`:
- `01_mallocCalloc.c` — `malloc` gives raw uninitialized memory, `calloc` gives
  zero-initialized memory of the same total size.
- `02_realloc.c` — growing a buffer on demand; `realloc` may move the block, so the
  returned pointer must always replace the old one, never assumed to equal it.
- `03_freeArrayOfStructs.c` — when a struct owns a `malloc`'d member (e.g. a `char *`
  name), freeing the containing array requires freeing each member first, then the
  array itself, in that order.

## 13. Enums

Not covered by dedicated notes — `code/08_Enum/`:
- `01_basicEnum.c` — default sequential values starting at 0 vs. explicit values (useful
  for protocol/status constants).
- `02_enumStateMachine.c` — an enum-driven state machine, dispatching on state via
  `switch` (see the project's `switch`/`case` indentation convention applied here).

## 14. Storage classes

Not covered by dedicated notes — `code/15_StorageClasses/` covers the classes not
already handled by the scope/linkage notes in section 5:
- `01_staticLocal.c` — a `static` local variable is initialized once and persists across
  calls, instead of being reinitialized on every call like an ordinary local.
- `02_staticFileScope.c` — a file-scope `static` restricts linkage to that translation
  unit only; no other `.c` file can reach it even with a matching `extern`. Contrast with
  `10_LinkerResolvesMultipleDefinition.c` from section 5, which covers symbols that *are*
  visible across translation units.
- `03_registerAuto.c` — `auto` is the (almost never written) default storage class for a
  block-scope variable; `register` is a hint the compiler may ignore, and you cannot take
  the address of a `register` variable.

## 15. Multi-file programs and headers

Not covered by dedicated notes — `code/27_MultiFile/` splits declarations from
definitions across files, the pattern this repo's own `common/utils.h` header uses for
sharing code between `05_BitManipulation/` files:
- `mathutils.h` — function declarations plus an `#ifndef`/`#define`/`#endif` include
  guard, so the header is safe to `#include` more than once in a build.
- `mathutils.c` — the definitions matching that header, compiled as its own translation
  unit (it has no `main`, so it can't be built as a standalone program — see the
  Makefile's special-cased link rule for `27_MultiFile/01_main`).
- `01_main.c` — `#include "mathutils.h"` and calls the functions; linking `01_main.c` and
  `mathutils.c` together produces one binary, demonstrating declaration (header) vs.
  definition (source) vs. linking (section 5) all in one place.

## 16. `const` and `volatile` qualifiers

Not covered by dedicated notes — `code/14_ConstVolatile/`:
- `01_constPointers.c` — the three distinct combinations: pointer-to-const (can't modify
  the pointee, can reassign the pointer), const-pointer (can modify the pointee, can't
  reassign the pointer), and const-pointer-to-const (neither).
- `02_volatile.c` — `volatile` tells the compiler a value can change outside the normal
  program flow (hardware register, signal handler, another thread), so every read/write
  must go to memory and can't be cached in a register or optimized away.

## 17. Command-line arguments

Not covered by dedicated notes — `code/16_CommandLineArgs/01_argcArgv.c`: `argv[0]` is
the program name itself, so real arguments start at `argv[1]`, and `argc` counts
`argv[0]` too.

## 18. Variadic functions

Not covered by dedicated notes — `code/17_Variadic/`:
- `01_miniPrintf.c` — a minimal `printf`-style formatter using `<stdarg.h>`
  (`va_list`/`va_start`/`va_arg`/`va_end`), showing how the real `printf` reads its
  argument list based on the format string.
- `02_sumVariadic.c` — a simpler variadic function driven by an explicit count argument
  instead of a format string; there is no way for a variadic function to know how many
  arguments it received without one of these mechanisms.

## 19. Runtime errors and signals

- **OS-delivered signals** on serious runtime errors (`SIGSEGV`, `SIGFPE`, `SIGILL`,
  `SIGBUS`, `SIGABRT`, `SIGSYS`, `SIGTRAP`), and how a signal (kernel → process) differs
  from an interrupt (process → kernel) — `06_Error_Signals.c`. Worth revisiting after
  section 7 (pointers) — most `SIGSEGV`s in practice come from bad pointer use.

## 20. File I/O and I/O system calls

- `code/26_FileIO/01_writeRead.c` — text mode: `fopen`/`fprintf`/`fgets`/`fclose`.
- `code/26_FileIO/02_binaryFreadFwrite.c` — binary mode: `fwrite`/`fread`.
- `code/26_FileIO/03_syscallIO.c` — raw I/O system calls: `open`/`read`/`write`/`close`/
  `lseek`, the unbuffered layer `fopen`/`fread` sit on top of.

## 21. Processes

Not covered by dedicated notes — `code/45_Processes/` (Linux/POSIX, `<unistd.h>`):
- `01_forkBasics.c` — `fork()` creates a near-identical copy of the calling process; the
  return value tells each side which one it is (`0` in the child, the child's PID in the
  parent). `waitpid()` blocks the parent until the child exits, and reports the child's
  exit status — without it, the two processes finish in an unpredictable order.
- `02_execFamily.c` — `fork()` + `execvp()` is how a shell launches a new program: `fork`
  makes a child, then `execvp` replaces that child's entire address space with a
  different program. A successful `exec*` call never returns to the calling code.

## 22. Threads

Not covered by dedicated notes — `code/49_Threads/` (POSIX threads, `<pthread.h>`; the
Makefile links these with `-pthread`):
- `01_pthreadBasics.c` — `pthread_create()`/`pthread_join()`, the thread equivalent of
  `fork()`/`waitpid()`. The key difference from processes (section 21): threads share
  one address space, so all threads see the same globals and heap directly, with no
  copy-on-write isolation.
- `02_mutex.c` — because threads share memory, `counter++` from multiple threads is a
  data race (the read-modify-write can interleave and lose increments); a
  `pthread_mutex_t` around the critical section makes each increment atomic relative to
  the other threads. Run this file's asserted expectation (`counter == expected`) as a
  before/after check by temporarily removing the lock calls to see the race in action.
- `03_conditionVariable.c` — a single-slot producer/consumer: `pthread_cond_wait()`
  atomically releases the mutex while blocked and reacquires it before returning, and
  both sides check their condition in a `while` loop (not `if`) to guard against spurious
  wakeups *and* against racing ahead before the other side has caught up — the producer
  must wait for `itemConsumed` just as much as the consumer waits for `itemReady`, or
  items get silently overwritten before they're ever read.
- `04_semaphore.c` — a counting semaphore (`sem_t`/`sem_wait`/`sem_post`) generalizes a
  mutex from "1 holder at a time" to "N holders at a time" — here, at most
  `MAX_CONCURRENT` worker threads hold a slot simultaneously.
- `05_deadlockAvoidance.c` — the classic deadlock shape (thread 1 holds A waits for B,
  thread 2 holds B waits for A, neither can proceed) and the simplest fix: every thread
  acquires shared locks in the same global order, so the circular-wait condition that
  causes deadlock can never form.

## 23. Inter-process communication (IPC) and process synchronization

Not covered by dedicated notes — `code/66_IPC/` builds directly on `fork()` from section
21:
- `01_pipeCommunication.c` — `pipe()` creates a one-way byte stream with a read end and a
  write end; a forked child inherits both ends, so closing the end it isn't using and
  writing/reading the other is the standard parent/child communication pattern.
- `02_sharedMemory.c` — `mmap()` with `MAP_SHARED | MAP_ANONYMOUS` gives a region that
  stays shared (not copy-on-write) across `fork()`, so a write the child makes is visible
  to the parent — unlike ordinary heap memory, which is private to each process after a
  fork.
- `03_processSemaphore.c` — the same `sem_t` from section 22's `04_semaphore.c`, but
  placed in `mmap`'d shared memory with `sem_init`'s `pshared` argument set nonzero, which
  is what makes a semaphore usable to synchronize *processes* (after a `fork()`) instead
  of only threads within one process — the parent blocks in `sem_wait()` until the child
  calls `sem_post()`, the process-level equivalent of the thread condition-variable
  handshake in section 22's `03_conditionVariable.c`.

## 24. Signal handling

Not covered by dedicated notes — `code/47_SignalHandling/` extends the signal *concepts*
from section 19 (which only covers signals the OS delivers automatically on an error)
with installing your own handlers:
- `01_customHandler.c` — `sigaction()` installs a handler for `SIGINT`; the handler only
  sets a `volatile sig_atomic_t` flag and returns immediately, since a handler can run at
  almost any point in the program and must not safely call non-reentrant functions like
  `printf`.
- `02_alarmRaise.c` — `alarm()` schedules a real `SIGALRM` after N seconds and `pause()`
  blocks until some signal arrives; `raise()` sends a signal to the current process
  itself, useful for triggering a handler on demand without waiting on a timer.

## 25. Linked lists

Not covered by dedicated notes — `code/29_LinkedList/` builds on structs (section 10)
and dynamic memory (section 12), one node at a time:
- `01_singlyLinkedList.c` — a self-referential `struct Node { ...; struct Node *next; }`,
  appending by walking to the last node's `next == NULL`, and `freeList` saving the
  `next` pointer before `free`-ing the current node (freeing first would make `next`
  dangling memory to read from).
- `02_reverseLinkedList.c` — reversing in place by walking the list once, re-pointing
  each node's `next` backward as you go (`prev`/`current`/`next` triple); this is the one
  place in the linked-list files where returning the new head means a genuine reference
  needs to change, matching the double-pointer-avoidance rule's carve-out from section 7.
- `03_doublyLinkedList.c` — adding a `prev` pointer alongside `next` enables backward
  traversal from any node (e.g. from the tail) without restarting from `head`, which a
  singly linked list can't do.
- `04_loopFinder.c` — Floyd's cycle detection (tortoise and hare): a slow pointer
  advancing one node at a time and a fast pointer advancing two will meet inside a loop
  if one exists; once they meet, resetting one pointer to `head` and advancing both one
  step at a time finds the loop's start node. `findLoopStartHash` solves the same problem
  by hashing each node's own pointer value into a visited set and returning the first
  node seen twice — O(n) time like Floyd's, but O(n) space instead of O(1), trading
  memory for a more direct "have I seen this node" check.

## 26. Binary trees

Not covered by dedicated notes — `code/30_BinaryTree/` extends the recursion intuition
from section 11 to a branching structure:
- `01_bstInsertSearch.c` — a binary search tree keeps every left-subtree value smaller
  and every right-subtree value larger than a node's own value, so both `insert` and
  `search` are O(log n) on a balanced tree (O(n) worst case if it degenerates into a
  chain) — the same divide-and-conquer idea as binary search (section 9), just on a
  linked structure instead of an array.
- `02_treeTraversals.c` — inorder (left, node, right — ascending order for a BST),
  preorder (node, left, right — useful for reconstructing structure), and postorder
  (left, right, node — useful for safely freeing children before their parent); all
  three are the same recursive shape with the `printf` moved to a different position.
- `03_heightAndLevelOrder.c` — `height()` as a recursive max-of-subtrees calculation, and
  `levelOrder()` as breadth-first traversal using an explicit array-backed queue —
  contrast with the depth-first traversals in `02_treeTraversals.c`, which rely on the
  call stack instead of an explicit queue.

## 27. Socket programming

- `code/69_SocketProgramming/01_tcpServer.c` — TCP server: `socket`/`bind`/`listen`/
  `accept`.
- `code/69_SocketProgramming/02_tcpClient.c` — TCP client: `socket`/`connect`, talking to
  the server over loopback. Run the server first, then the client, in separate
  terminals.

## 28. `_Generic` keyword

- `code/28_Generics/01_genericMacro.c` — compile-time type dispatch with `_Generic`, used
  to fake function overloading and to write a type-name macro.

## 29. Heaps

`Notes/12_Heap.c` covers the basic definition (min-heap vs. max-heap, the array-index
parent/child formulas, and why a heap can't be binary-searched) before the code:
- `code/31_Heap/01_minHeap.c` — a binary min-heap stored in a flat array (child at
  `2i+1`/`2i+2`, parent at `(i-1)/2` — no explicit pointers, unlike the BST in
  `30_BinaryTree/`); `push` bubbles a new element up, `pop` removes the root and bubbles
  the replacement down, both O(log n).
- `code/31_Heap/02_heapSort.c` — heap sort: build a max-heap in place, then repeatedly
  swap the root to the end of the shrinking heap and re-sift — O(n log n), in place, no
  extra array.

## 30. Graphs

`Notes/13_Graph.c` covers the basic definition (vertices/edges, directed vs. undirected,
weighted vs. unweighted, and how a graph relates to the trees in section 26) before the
code:
- `code/32_Graph/01_adjacencyListBFS.c` — graph as an adjacency list (array of per-vertex
  linked lists); breadth-first search with an explicit queue, marking vertices `visited`
  to handle cycles (which trees, in section 26, don't have to worry about).
- `code/32_Graph/02_adjacencyMatrixDFS.c` — graph as an adjacency matrix (O(1) edge check,
  O(V²) memory regardless of edge count — contrast with the adjacency list's O(degree)
  check but edge-proportional memory); depth-first search recursing via the call stack
  instead of BFS's explicit queue.
- `code/32_Graph/03_dijkstraShortestPath.c` — Dijkstra's shortest path: repeatedly
  finalize the closest unvisited vertex and relax its neighbors' distances through it;
  only correct with non-negative edge weights. `minDistanceVertex`'s linear scan is the
  same "take the cheapest next option" idea as popping from the min-heap in section 29 —
  a real implementation would use that heap instead of an O(V) scan.

## 31. Tries

`Notes/14_Trie.c` covers the basic definition (one child slot per possible next
character, shared prefixes stored once, and the difference between a node existing at
all vs. `isEndOfWord` being set) before the code:
- `code/33_Trie/01_insertSearch.c` — `insert` walking one character at a time, creating
  nodes only where the path doesn't already exist; `search` requires both reaching the
  last character's node *and* `isEndOfWord` being set — a node existing only means some
  inserted word passes through it, not that this exact path is itself a complete word.
- `code/33_Trie/02_prefixSearch.c` — `startsWith`, the trie's signature use case
  (autocomplete/spell-check): just walking a path and checking it exists, without
  needing `isEndOfWord` on the final node.

## 32. Dynamic programming

`Notes/15_DynamicProgramming.c` covers the basic definition (optimal substructure,
overlapping sub-problems, and top-down/memoization vs. bottom-up/tabulation) before the
code:
- `code/34_DynamicProgramming/01_fibMemoVsTabulation.c` — the same fibonacci problem
  solved both ways side by side; compare against the naive O(2^n) recursion in
  `12_Recursion/02_fibonacci.c` to see exactly what memoization is fixing.
- `code/34_DynamicProgramming/02_knapsack.c` — a 2D DP table (items considered ×
  remaining capacity), each cell choosing the better of "skip this item" vs. "take it".
- `code/34_DynamicProgramming/03_longestCommonSubsequence.c` — a 2D DP table over two
  strings' positions instead of items/capacity, showing the same table-filling pattern
  applies once you identify what two numbers describe a sub-problem.

## 33. System calls

`Notes/16_SystemCalls.c` covers the basic definition (crossing into kernel-space,
libc wrappers vs. raw syscalls, and errno-based failure reporting) and groups every
syscall example in this repo by category:
- `code/44_SystemCalls/01_libcWrapperVsRawSyscall.c` — `getpid()` vs.
  `syscall(SYS_getpid)`, showing a libc wrapper and the raw syscall interface doing the
  same thing.
- `code/44_SystemCalls/02_errnoAndPerror.c` — `errno`/`perror`; save `errno` into a local
  immediately after a failing call, since another library call in between (even a
  successful one) can overwrite it.
- **File system**: `code/26_FileIO/03_syscallIO.c` (`open`/`read`/`write`/`close`/
  `lseek` — file *contents*) and `code/44_SystemCalls/03_fileSystemCalls.c`
  (`stat`/`mkdir`/`rmdir`/`creat`/`unlink` — file *metadata and directory entries*).
- **Process control**: `code/45_Processes/01_forkBasics.c` (`fork`/`waitpid`) and
  `code/45_Processes/02_execFamily.c` (`execvp`) — section 21.
- **Memory management**: `code/44_SystemCalls/05_memoryManagementMprotect.c` —
  `mprotect` changes an already-mapped region's page permissions; writing to a page
  after marking it read-only faults with `SIGSEGV`. See also section 34 (paging) and
  section 12 (dynamic memory).
- **IPC**: `code/66_IPC/` (`pipe`, `mmap`+`MAP_SHARED`, process-shared semaphores) —
  section 23.
- **Device management**: `code/44_SystemCalls/04_deviceManagementIoctl.c` — `ioctl`, the
  catch-all syscall for device-specific requests that don't fit the read/write model
  (here, asking a terminal for its window size).

## 34. Paging

`Notes/17_Paging.c` covers the basic definition (fixed-size pages, virtual-to-physical
translation via page tables, per-page protection bits, and demand paging) before the
code:
- `code/57_Paging/01_pageSizeAndMmap.c` — querying the runtime page size
  (`sysconf(_SC_PAGESIZE)`) and `mmap`'ing a multi-page region, one byte written per page.
- `code/57_Paging/02_demandPaging.c` — reading this process's own resident set size (RSS)
  from `/proc/self/status` before an `mmap`, right after it (barely changes — no physical
  memory assigned yet), and after touching every page (grows roughly one page at a time)
  — demand paging made directly observable.

## 35. `qsort`/`bsearch` and control-flow escapes

`Notes/18_GenericAlgorithmsAndControlFlowEscapes.c` covers both clusters below:
- `code/19_QsortBsearch/01_qsortInts.c` — `qsort`'s comparator does all the type-specific
  work, the same `void *` type-erasure idea as `03_pointers/06_voidPointer.c` applied to a
  whole algorithm.
- `code/19_QsortBsearch/02_bsearchStructs.c` — `bsearch` on structs, requiring the array
  pre-sorted with the *same* comparator it's given.
- `code/18_ControlFlowExtras/01_goto.c` — breaking out of nested loops in one jump, `goto`'s
  one broadly accepted use.
- `code/18_ControlFlowExtras/02_assert.c` — checking an invariant, not validating real
  (possibly-wrong) input; compiled out entirely under `NDEBUG`.
- `code/18_ControlFlowExtras/03_setjmpLongjmp.c` — unwinding the call stack straight back
  to a matching `setjmp`, C's closest thing to an exception.

## 36. Time, math, qualifiers, and wide characters

`Notes/19_TimeMathQualifiersAndWideChars.c` covers all of the below:
- `code/35_TimeAndMath/01_timeBasics.c` — `time`/`localtime`/`strftime` vs. `clock`
  (CPU time, not wall-clock time).
- `code/35_TimeAndMath/02_mathFunctions.c` — `sqrt`/`pow`/`floor`/`ceil`/`fabs`/trig
  (radians, not degrees), and `isnan` as the only correct way to test for `NAN`.
- `code/21_RestrictQualifier/01_restrict.c` — `restrict` promises the compiler two
  pointer parameters never overlap, the same assumption `memcpy` relies on.
- `code/23_InlineFunctions/01_inline.c` — `inline` as a hint the compiler may ignore,
  like `register` in section 14.
- `code/22_WideChars/01_wcharBasics.c` — `wchar_t` for characters outside a plain
  `char`'s one-byte range.

## 37. Concurrency and memory-layout extras

`Notes/20_ConcurrencyAndMemoryLayoutExtras.c` covers all of the below:
- `code/52_Atomics/01_stdatomicCounter.c` — `atomic_int`/`atomic_fetch_add` as a
  lock-free alternative to the mutex-protected counter in section 22
  (`49_Threads/02_mutex.c`).
- `code/52_Atomics/02_threadLocalStorage.c` — `_Thread_local` gives each thread its own
  copy, so there's nothing shared to race on.
- `code/20_AdvancedArrays/01_variableLengthArray.c` — a runtime-sized, stack-allocated
  array; no `malloc` failure check is possible, unlike section 12.
- `code/20_AdvancedArrays/02_flexibleArrayMember.c` — a struct's variable-length payload
  allocated in the same block as its fixed fields, contributing 0 to `sizeof(struct)`.
- `code/25_BitFields/01_bitFieldStruct.c` — claiming exactly N bits per struct member,
  at the cost of implementation-defined bit order/padding (contrast with the portable
  shifting/masking in section 8).

## 38. System V IPC, CLI parsing, and process scheduling

`Notes/21_SystemVIpcCliAndScheduling.c` covers all of the below:
- `code/67_SystemVIPC/01_sharedMemorySegment.c` — `shmget`/`shmat`/`shmctl(IPC_RMID)`, a
  key-based, kernel-owned alternative to the POSIX `mmap` region in section 23.
- `code/67_SystemVIPC/02_messageQueue.c` — `msgget`/`msgsnd`/`msgrcv`, type-tagged
  messages instead of a pipe's undifferentiated byte stream.
- `code/67_SystemVIPC/03_semaphoreSet.c` — `semget`/`semop`/`semctl` operating on a
  *set* of semaphores by index, vs. the single POSIX `sem_t` in section 23.
- `code/24_CommandLineOptions/01_getoptBasics.c` — `getopt`/`optarg`/`optind` flag
  parsing, built on the raw `argc`/`argv` from section 17.
- `code/53_ProcessScheduling/01_niceAndProcInspection.c` — `nice()` as a scheduler
  priority hint, and reading `/proc/self/status` for this process's own state (RSS was
  already read from the same file in section 34).

## 39. Classic synchronization problems

`Notes/22_SynchronizationProblems.c` covers all three:
- `code/50_SyncProblems/01_boundedBufferProducerConsumer.c` — a multi-slot buffer
  extending the single-slot handoff in section 22 (`49_Threads/03_conditionVariable.c`).
- `code/50_SyncProblems/02_readerWriter.c` — many readers may overlap safely; a writer
  needs full exclusive access; only the first reader in/last reader out touches the
  writer-exclusion lock.
- `code/50_SyncProblems/03_diningPhilosophers.c` — the textbook circular-wait deadlock
  (same shape as section 22's `05_deadlockAvoidance.c`, five participants instead of
  two), fixed by having one participant acquire locks in the opposite order.

## 40. CPU scheduling algorithms

`Notes/23_CPUSchedulingAlgorithms.c` covers all four, each tracked via
waiting-time/turnaround-time:
- `code/54_CPUScheduling/01_fcfs.c` — First-Come, First-Served; simplest policy, suffers
  the "convoy effect".
- `code/54_CPUScheduling/02_sjf.c` — Shortest Job First; minimizes average waiting time,
  requires knowing burst times in advance.
- `code/54_CPUScheduling/03_roundRobin.c` — fixed time-slice preemption; bounds worst-case
  wait, costs more context switches.
- `code/54_CPUScheduling/04_priorityScheduling.c` — always run the highest-priority ready
  process; structurally the same loop as SJF, comparing priority instead of burst time.

## 41. Memory management algorithms

`Notes/24_MemoryManagementAlgorithms.c` covers both families, extending section 34
(paging):
- `code/56_MemoryManagement/01_pageReplacement.c` — FIFO vs. LRU page replacement on the
  same reference string; LRU is not guaranteed to beat FIFO on every string, and this
  example demonstrates that directly.
- `code/56_MemoryManagement/02_contiguousAllocation.c` — First Fit vs. Best Fit block
  allocation, comparing resulting fragmentation and allocation failures.

## 42. Process lifecycle edge cases and memory-mapping extras

`Notes/25_ProcessLifecycleAndMemoryExtras.c` covers all four:
- `code/46_ProcessLifecycle/01_zombieProcess.c` — a child that exited but hasn't been
  `wait()`-ed on yet; visible as state `Z` in `ps` until reaped.
- `code/46_ProcessLifecycle/02_orphanProcess.c` — a child whose parent exits first gets
  re-parented to init/PID 1; `getppid()` visibly changes.
- `code/46_ProcessLifecycle/03_daemonProcess.c` — the double-fork + `setsid()` sequence
  that detaches a process from its controlling terminal entirely.
- `code/59_MmapFile/01_fileBackedMmap.c` — mapping a real file descriptor (not anonymous,
  contrast with section 34) so reads/writes through the pointer act directly on the
  file's contents.
- `code/60_CopyOnWrite/01_forkCow.c` — physical pages stay shared and read-only after
  `fork()` until a write triggers copy-on-write, using the same page-fault mechanism as
  demand paging (section 34).

## 43. Lock-free ring buffer and false sharing

`Notes/26_SeniorCInterviewQuestions.c` covers the concepts behind both files:
- `code/61_LockFreeRingBuffer/01_spscRingBuffer.c` — a single-producer/single-consumer
  ring buffer using plain atomic loads/stores (no CAS loop needed, since each index has
  exactly one writer) instead of the mutex + condition variables in section 39's
  `50_SyncProblems/01_boundedBufferProducerConsumer.c`.
- `code/62_FalseSharing/01_falseSharingBenchmark.c` — two per-thread counters sharing a
  cache line slow each other down purely from layout, with zero logical contention;
  padding each counter to its own cache line measurably fixes it. Relies on this repo's
  Makefile building without `-O2`/`-O3` — see the file's comment on why.

## 44. Custom allocators

`Notes/26_SeniorCInterviewQuestions.c` covers both:
- `code/65_CustomAllocator/01_arenaAllocator.c` — a bump allocator over one static
  buffer; O(1) allocation (a pointer add), but only frees everything at once via
  `arenaReset()` — the right tool when a batch of allocations share one lifetime.
- `code/65_CustomAllocator/02_fixedSizePoolAllocator.c` — a free list threaded directly
  through the unused fixed-size blocks themselves (no separate bookkeeping array); O(1)
  allocation *and* individual free, for repeated same-size allocations.

## 45. Strict aliasing violation

`Notes/26_SeniorCInterviewQuestions.c` covers the rule; the code demonstrates it directly:
- `code/42_StrictAliasing/01_strictAliasingViolation.c` — writing through an `int *` then
  a `float *` to the same address, and reading back through the `int *`, returns the
  stale value `10` at `-O2` (the compiler assumes unrelated pointer types can't alias)
  but the actual reinterpreted bits at `-O0` (this repo's Makefile default) — the same
  divergence, and the same reason, as section 43's false-sharing benchmark being
  optimization-level-dependent. `memcpy`-based reinterpretation is the safe fix, shown in
  the same file.

## 46. I/O multiplexing (select/poll/epoll)

`Notes/26_SeniorCInterviewQuestions.c` covers the tradeoffs; all three watch the same two
pipes:
- `code/71_IOMultiplexing/01_selectMultiplePipes.c` — `select()`'s fixed-size `fd_set`
  bitmask, rebuilt on every call.
- `code/71_IOMultiplexing/02_pollMultiplePipes.c` — `poll()`'s array of `{fd, events}`,
  no fixed-size cap.
- `code/71_IOMultiplexing/03_epollServer.c` — `epoll_create1`/`epoll_ctl`/`epoll_wait`;
  the kernel remembers the watch list across calls instead of it being passed in full
  every time, the reason real servers use epoll at scale.

## 47. UDP sockets

`Notes/26_SeniorCInterviewQuestions.c` covers TCP vs. UDP, Nagle's algorithm, and
`SO_REUSEADDR`/`SO_REUSEPORT`:
- `code/70_UDPSockets/01_udpServer.c`/`02_udpClient.c` — `SOCK_DGRAM`,
  `sendto`/`recvfrom` with no `connect()`/`accept()`, contrasting directly with the TCP
  pair in `69_SocketProgramming/`.

## 48. Security demonstrations

`Notes/26_SeniorCInterviewQuestions.c` covers all three:
- `code/77_SecurityDemos/01_bufferOverflowStackCanary.c` — an unchecked `strcpy`
  overflow; `-fstack-protector-all` catches it with "stack smashing detected", the
  default build here does not (raw `SIGSEGV`) — see the file's comment.
- `code/77_SecurityDemos/02_formatStringVulnerability.c` — passing user input as
  `printf`'s format string directly leaks stack/register contents via `%x`.
- `code/77_SecurityDemos/03_integerOverflowToBufferOverflow.c` — a size multiplication
  that wraps before reaching `malloc`, undersizing the buffer relative to the caller's
  actual intent.

## 49. ThreadSanitizer

`Notes/26_SeniorCInterviewQuestions.c` covers the tool:
- `code/78_ThreadSanitizerDemo/01_racyCounter.c` — deliberately unsynchronized on
  purpose; compile with `-fsanitize=thread -pthread` to see the exact race reported.

## 50. Dynamic loading and symbol visibility

`Notes/26_SeniorCInterviewQuestions.c` covers both:
- `code/74_DynamicLoading/01_loader.c`/`plugin.c` — `dlopen`/`dlsym`/`dlclose` loading a
  shared library built with `-fvisibility=hidden`; only the symbol marked
  `__attribute__((visibility("default")))` is reachable via `dlsym`.

## 51. `extern "C"` (C/C++ interop)

`Notes/26_SeniorCInterviewQuestions.c` covers name mangling:
- `code/75_ExternC/mathutils.h`/`.c`/`01_main.cpp` — a C++ caller linking against a
  C-compiled TU through `extern "C"` declarations guarded by `#ifdef __cplusplus`.

## 52. Resource limits

`Notes/26_SeniorCInterviewQuestions.c` covers `ulimit`/fork bombs conceptually (not run):
- `code/76_ResourceLimits/01_rlimitFileDescriptors.c` — `getrlimit`/`setrlimit` on
  `RLIMIT_NOFILE`, deliberately lowered and then hit, producing `EMFILE`.

## 53. Opaque pointers and vtable emulation

`Notes/26_SeniorCInterviewQuestions.c` covers both design patterns:
- `code/36_OpaquePointer/handle.h`/`.c`/`01_main.c` — a PIMPL-equivalent: callers only
  ever see a pointer to an incomplete type, never the real struct fields.
- `code/37_VTableEmulation/01_shapeVtable.c` — a hand-rolled struct-of-function-pointers
  vtable, close to what a C++ compiler generates for virtual dispatch.

## 54. Unit testing in C

`Notes/26_SeniorCInterviewQuestions.c` covers `-Werror` and testing philosophy:
- `code/38_UnitTesting/01_assertBasedTests.c` — a minimal `EXPECT_EQ`-style test macro
  that records every failure instead of aborting on the first one, unlike a raw
  `assert()` (section 35).

## Suggested order for a first pass

1. Sections 1–3 (fundamentals, compilation, data types)
2. Section 4 (loops) → Section 6 (arrays) — loops are how arrays get filled/read
3. Section 5 (functions/scope/linking) → Section 11 (recursion) — recursion is just
   functions calling themselves, so it builds directly on section 5
4. Section 7 (pointers) → Section 8 (bit manipulation) → Section 12 (dynamic memory) —
   `malloc`/`realloc`/`free` only make sense once pointer intuition is solid
5. Section 9 (algorithms on arrays) as applied practice
6. Section 10 (structs/unions/strings) → Section 13 (enums) once pointers (section 7)
   feel comfortable — `->` and array-of-struct patterns build directly on pointer/array
   intuition
7. Section 14 (storage classes) → Section 15 (multi-file programs) — `static`/`extern`
   only matter once you're splitting code across translation units
8. Section 16 (`const`/`volatile`) → Section 17 (command-line args) → Section 18
   (variadic functions) as smaller, self-contained topics
9. Section 19 (signals) as a wrap-up on what happens when the above goes wrong
10. Section 20 (file I/O) as a capstone that combines pointers, dynamic memory, and
    error checking (`NULL`/EOF) from everything above
11. Section 21 (processes) → Section 22 (threads) — both need section 5 (functions) and
    section 12 (dynamic memory/pointers) as background; do processes first since threads
    are usually explained by contrast with them (shared vs. private address space)
12. Section 23 (IPC) right after section 21 — pipes and shared memory only make sense
    once `fork()` is understood
13. Section 24 (signal handling) last, after section 19 (signals) — you're now writing
    the handler side of the same mechanism that section 19 only observed from outside
14. Section 25 (linked lists) once section 10 (structs) and section 12 (dynamic memory)
    both feel solid — a linked list is just a struct that points to another instance of
    itself, allocated one node at a time
15. Section 26 (binary trees) right after linked lists — same self-referential-struct
    idea, but branching, and leans heavily on section 11's recursion intuition
16. Section 27 (sockets) after section 23 (IPC) — sockets are just another form of
    inter-process communication, over a network stack instead of pipes/shared memory
17. Section 28 (`_Generic`) any time after section 16 (`const`/`volatile`) — a
    small, self-contained language feature with no other prerequisites
18. Section 29 (heaps) once section 9 (algorithms/arrays) feels solid — a heap is an
    array with implicit tree structure, so array intuition transfers directly
19. Section 30 (graphs) right after section 26 (binary trees) — BFS/DFS extend the tree
    traversals directly, generalized to structures with cycles
20. Section 31 (tries) right after section 26 (binary trees) — another tree variant,
    branching on characters instead of a two-way comparison, and leaning on section 10's
    strings
21. Section 32 (dynamic programming) once section 11 (recursion) feels solid —
    memoization is a direct, small addition to recursion you already know
22. Section 33 (system calls) after section 21 (processes) and section 23 (IPC) — it's
    the unifying category those two (plus the file I/O in section 20) already sit under,
    now filled out with file-metadata, device-management, and memory-management syscalls
23. Section 34 (paging) right after section 33 (system calls) and section 12 (dynamic
    memory) — makes concrete what `mmap`/`mprotect` are actually doing underneath
24. Section 35 (`qsort`/`bsearch`/control-flow escapes) once section 7 (pointers,
    specifically function pointers) feels solid
25. Section 36 (time/math/qualifiers/wide chars) any time — all self-contained,
    no dependency on anything above
26. Section 37 (concurrency/memory-layout extras) after section 22 (threads) and
    section 12 (dynamic memory) — each item directly extends one of those two
27. Section 38 (System V IPC/CLI/scheduling) right after section 23 (IPC) and
    section 17 (command-line args)
28. Section 39 (synchronization problems) right after section 22 (threads) — each
    problem is a direct, slightly harder variant of that section's mutex/condition-
    variable/deadlock material
29. Section 40 (CPU scheduling algorithms) any time after section 38's `nice()` — makes
    concrete what a scheduler actually decides, beyond the priority hint alone
30. Section 41 (memory management algorithms) right after section 34 (paging) — extends
    "what a page is" to "how the OS decides what to evict or allocate"
31. Section 42 (process lifecycle/mmap extras) right after section 21 (processes) and
    section 34 (paging) — edge cases and follow-on mmap/COW material for both
32. Section 43 (lock-free ring buffer/false sharing) last of the concurrency material —
    both assume section 22 (threads) and section 37 (atomics) are already comfortable
33. Section 44 (custom allocators) any time after section 12 (dynamic memory) — both
    are alternatives to plain `malloc`/`free` for a specific allocation pattern
34. Section 45 (strict aliasing) any time after section 6 (pointers/`06_voidPointer.c`)
    and section 16 (`const`/`volatile`, for the general "compiler assumptions" theme)
35. Section 46 (I/O multiplexing) → section 47 (UDP) after section 23 (IPC, for pipes)
    and section 27 (TCP sockets) — both extend the socket/descriptor material there
36. Section 48 (security demos) after section 7 (pointers) and section 10 (strings) —
    every one of these bugs is a pointer/buffer-bounds mistake at heart
37. Section 49 (ThreadSanitizer) right after section 22 (threads) — the tool for
    catching exactly the race-condition mistakes that section demonstrates by hand
38. Section 50 (dynamic loading) → section 51 (`extern "C"`) after section 15
    (multi-file programs) — both are advanced linking topics building on it
39. Section 52 (resource limits) any time after section 21 (processes)
40. Section 53 (opaque pointers/vtables) after section 10 (structs) and section 7
    (function pointers, for the vtable half)
41. Section 54 (unit testing) any time after section 35 (`assert`)

## Gaps not yet covered by this repo

Common C topics still without notes or examples here — worth adding as you learn them,
following the existing numbering/naming convention:
- `sched_setscheduler` and other scheduling-policy calls beyond the `nice()` hint in
  section 38
- `<stdint.h>` fixed-width integer types (`int32_t`, `uint64_t`, etc.)
- Designated initializers and compound literals (C99)
- `alignas`/`alignof` (C11) and `_Static_assert`

