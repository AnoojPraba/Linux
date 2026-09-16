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

Not covered by dedicated notes — the `code/03_array/` examples form the progression:
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

Not covered by dedicated notes — `code/pointers/` builds the concept in two steps:
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

`code/BitManipulation/` is a self-contained mini-module (shares `utils.h` for
hex/decimal input parsing, see the project CLAUDE.md):
1. `02_Boolean.c` — using bitwise ops for boolean-style logic.
2. `03_OddOrEven.c` — checking a bit (`n & 1`) to classify a number.
3. `01_BitManipulation.c` — general set/clear/toggle/check-bit operations.
4. `04_get_bitset.c` — the most complete example: parses hex or decimal input, prints the
   full binary representation with bit-position ruler (`printBinary`), and prints
   compressed ranges of set bits (e.g. `[0-3 5 7-9]`) via `bitSet`.

## 9. Searching, sorting, and patterns

- `code/04_Search_alg/02_LinearSearch.c` — the baseline O(n) search: check every element,
  no ordering assumption. Start here before the others.
- `code/04_Search_alg/03_BinarySearchIterative.c` — O(log n) search on a *sorted* array,
  halving the search range each iteration; compare against the recursive version in
  section 11 (`09_Recursion/03_recursiveBinarySearch.c`) to see the same logic with and
  without a call stack.
- `code/04_Search_alg/01_Second_Largest.c` — single-pass second-largest-in-array by
  tracking two running values (`max`, `sec_max`) instead of sorting first; a good example
  of trading a full sort (O(n log n)) for a linear scan (O(n)) when you only need one
  statistic.
- `code/06_Sorting/Simplesort.cpp` — a basic sort implementation for comparison (note:
  this one is C++, not C — useful to see how similar the syntax is for simple algorithms).
- `code/05_pattern/Pattern.c` — nested-loop pattern printing (triangles, etc.); a good
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
- `code/08_Strings/01_stringBasics.c` — a C string is a `char` array terminated by `\0`;
  `strlen`/`strcpy`/`strcat`/`strcmp`/`strchr` from `<string.h>`.
- `code/08_Strings/02_manualStringFunctions.c` — hand-rolled `strlen`/`strcpy`/`strrev` to
  see what the library functions are doing under the hood.
- `code/08_Strings/03_tokenizeString.c` — `strtok` splits a string in place (it overwrites
  each delimiter with `\0`), which is why later calls pass `NULL` to continue the same
  tokenization.

## 11. Recursion

Not covered by dedicated notes — `code/09_Recursion/` builds the concept in three steps:
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

Not covered by dedicated notes — `code/10_DynamicMemory/` extends the single
`malloc`/`free` pair already seen in `pointers/04_doublePointer.c`:
- `01_mallocCalloc.c` — `malloc` gives raw uninitialized memory, `calloc` gives
  zero-initialized memory of the same total size.
- `02_realloc.c` — growing a buffer on demand; `realloc` may move the block, so the
  returned pointer must always replace the old one, never assumed to equal it.
- `03_freeArrayOfStructs.c` — when a struct owns a `malloc`'d member (e.g. a `char *`
  name), freeing the containing array requires freeing each member first, then the
  array itself, in that order.

## 13. Enums

Not covered by dedicated notes — `code/12_Enum/`:
- `01_basicEnum.c` — default sequential values starting at 0 vs. explicit values (useful
  for protocol/status constants).
- `02_enumStateMachine.c` — an enum-driven state machine, dispatching on state via
  `switch` (see the project's `switch`/`case` indentation convention applied here).

## 14. Storage classes

Not covered by dedicated notes — `code/13_StorageClasses/` covers the classes not
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

Not covered by dedicated notes — `code/14_MultiFile/` splits declarations from
definitions across files, the pattern this repo's own `common/utils.h` header uses for
sharing code between `BitManipulation/` files:
- `mathutils.h` — function declarations plus an `#ifndef`/`#define`/`#endif` include
  guard, so the header is safe to `#include` more than once in a build.
- `mathutils.c` — the definitions matching that header, compiled as its own translation
  unit (it has no `main`, so it can't be built as a standalone program — see the
  Makefile's special-cased link rule for `14_MultiFile/01_main`).
- `01_main.c` — `#include "mathutils.h"` and calls the functions; linking `01_main.c` and
  `mathutils.c` together produces one binary, demonstrating declaration (header) vs.
  definition (source) vs. linking (section 5) all in one place.

## 16. `const` and `volatile` qualifiers

Not covered by dedicated notes — `code/15_ConstVolatile/`:
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

- `code/11_FileIO/01_writeRead.c` — text mode: `fopen`/`fprintf`/`fgets`/`fclose`.
- `code/11_FileIO/02_binaryFreadFwrite.c` — binary mode: `fwrite`/`fread`.
- `code/11_FileIO/03_syscallIO.c` — raw I/O system calls: `open`/`read`/`write`/`close`/
  `lseek`, the unbuffered layer `fopen`/`fread` sit on top of.

## 21. Processes

Not covered by dedicated notes — `code/18_Processes/` (Linux/POSIX, `<unistd.h>`):
- `01_forkBasics.c` — `fork()` creates a near-identical copy of the calling process; the
  return value tells each side which one it is (`0` in the child, the child's PID in the
  parent). `waitpid()` blocks the parent until the child exits, and reports the child's
  exit status — without it, the two processes finish in an unpredictable order.
- `02_execFamily.c` — `fork()` + `execvp()` is how a shell launches a new program: `fork`
  makes a child, then `execvp` replaces that child's entire address space with a
  different program. A successful `exec*` call never returns to the calling code.

## 22. Threads

Not covered by dedicated notes — `code/19_Threads/` (POSIX threads, `<pthread.h>`; the
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

Not covered by dedicated notes — `code/20_IPC/` builds directly on `fork()` from section
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

Not covered by dedicated notes — `code/21_SignalHandling/` extends the signal *concepts*
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

Not covered by dedicated notes — `code/22_LinkedList/` builds on structs (section 10)
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

## 26. Binary trees

Not covered by dedicated notes — `code/23_BinaryTree/` extends the recursion intuition
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

- `code/24_SocketProgramming/01_tcpServer.c` — TCP server: `socket`/`bind`/`listen`/
  `accept`.
- `code/24_SocketProgramming/02_tcpClient.c` — TCP client: `socket`/`connect`, talking to
  the server over loopback. Run the server first, then the client, in separate
  terminals.

## 28. `_Generic` keyword

- `code/25_Generics/01_genericMacro.c` — compile-time type dispatch with `_Generic`, used
  to fake function overloading and to write a type-name macro.

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

## Gaps not yet covered by this repo

Common C topics still without notes or examples here — worth adding as you learn them,
following the existing numbering/naming convention:
- `getopt`-style command-line option parsing (section 17 covers raw `argc`/`argv` only)
- `qsort`/`bsearch` from `<stdlib.h>` (function pointers, covered in
  `pointers/05_functionPointer.c`, are the prerequisite)
- Bit-fields in structs
- `errno` and `perror` for standard-library error reporting
- System V IPC (`shmget`/`msgget`/`semget`) as an alternative to the POSIX `mmap`/pipe
  approach in `code/20_IPC/`
- Process scheduling/priority (`nice`, `sched_setscheduler`) and `/proc` inspection
