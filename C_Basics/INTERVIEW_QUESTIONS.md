# Interview Questions

A companion question bank, not a duplicate of `code/*/NOTES.md`. This collects the KINDS
of questions interviewers actually ask - scenario/debugging, "what's wrong with this
code", design tradeoffs, embedded-practical, and explain-it-simply - each cross-referenced
to the folder under `code/` where the underlying material lives. Use the NOTES.md files
for depth; use this file to rehearse how the question actually gets phrased.

## 1. Scenario-based / debugging questions

- **A program works in debug builds but crashes in release builds - what do you check
  first?** Debug builds often zero-fill stack memory, masking an uninitialized-read bug
  that release optimization exposes. Also check optimizer reordering/elision around UB
  (e.g. removed overflow checks). See `57_UndefinedBehaviorCatalog`, `69_PerfAndStrace`.
- **A colleague says "adding `-O2` fixed the crash" - what does that actually tell you,
  and does it fix anything?** It tells you the bug is likely UB-dependent (uninitialized
  read, aliasing violation, overflow) whose symptom changed with codegen - it did not fix
  the bug, it just moved when/whether it manifests. See `57_UndefinedBehaviorCatalog`.
- **This function works for small inputs but corrupts memory for large ones - how do you
  find the bug?** Walk through buffer-size assumptions, reach for ASan/Valgrind rather
  than guessing, and reproduce with a sanitized build first. See `65_SecurityDemos`,
  `68_ValgrindAndAsan`.

  ```c
  /* BUG (illustration only, not compiled): no bound on inputLen, so a large */
  /* input silently writes past dest and corrupts adjacent stack memory.    */
  /*
   * void copyIn(char *dest, const char *src, size_t inputLen)
   * {
   *     size_t i;
   *
   *     for (i = 0; i < inputLen; i++)
   *     {
   *         dest[i] = src[i];
   *     }
   * }
   */

  /* FIX: clamp the copy to the destination's real capacity. */
  void copyInFixed(char *dest, size_t destCap, const char *src, size_t inputLen)
  {
      size_t i;
      size_t limit;

      limit = (inputLen < destCap - 1) ? inputLen : destCap - 1;
      for (i = 0; i < limit; i++)
      {
          dest[i] = src[i];
      }
      dest[limit] = '\0';
  }
  /* copyInFixed("this input is way longer than smallBuf", ...) into an 8-byte */
  /* buffer prints "this in" instead of overrunning the buffer. Verified with  */
  /* gcc -std=c99 -Wall -Wextra. */
  ```
- **A multithreaded program passes every test but fails once a week in production - how
  do you approach it?** Talk about data races being timing-dependent and untestable by
  inspection alone; reach for ThreadSanitizer/Helgrind rather than adding sleeps to "fix"
  it. See `66_ThreadSanitizerDemo`, `68_ValgrindAndAsan`.
- **Walk me through how you'd use gdb to find why this pointer is NULL at line 40.**
  Expect a candidate to mention breakpoints, `bt`, `print`, `watch`, and stepping into the
  call chain rather than adding printf statements blindly. See `67_GdbWorkflow`.
- **A heap corruption bug only shows up after thousands of allocations - what's your
  triage order?** ASan/Valgrind first to get a precise crash site, then gdb to inspect
  state, then perf/strace only if it's a performance-adjacent issue. See
  `70_CombinedDebuggingCaseStudy`, `68_ValgrindAndAsan`.
- **A function is UB but "seems to always work" on your machine - why is that not a
  green light to ship it?** Different compiler, flags, or even an unrelated code change
  elsewhere in the TU can change codegen and break it later; UB has no behavior contract
  at all. See `57_UndefinedBehaviorCatalog`.

## 2. "What's wrong with this code?" snippets

- **A function returns `&localVar` (a pointer to a stack-local variable).** The stack
  frame is reclaimed on return; the pointer dangles and reading through it is UB even if
  it "looks fine" momentarily. See `57_UndefinedBehaviorCatalog`.

  ```c
  /* BUG (not compiled/kept in repo): returns address of a stack-local variable. */
  /*
   * int *makeValueBuggy(void)
   * {
   *     int localVar = 42;
   *     return &localVar;   // dangling: localVar's frame is gone on return
   * }
   */

  /* FIX: caller-owned heap allocation instead of a stack address. */
  int *makeValueFixed(int value)
  {
      int *heapVal;

      heapVal = malloc(sizeof(*heapVal));
      if (heapVal == NULL)
      {
          return NULL;
      }
      *heapVal = value;
      return heapVal;
  }
  /* Verified with gcc -std=c99 -Wall -Wextra: prints "value = 42". */
  ```
- **`for (int i = 0; i <= size; i++) arr[i] = 0;`** Off-by-one: the loop bound should be
  `<`, not `<=`, writing one element past the end of `arr`. See `04_array`.
- **`for (unsigned int i = 10; i >= 0; i--)`** Comparing/decrementing an unsigned index
  below zero wraps to a huge positive value instead of going negative - an infinite loop.
  Ask the candidate to spot the signed/unsigned mismatch. See `05_BitManipulation`,
  `57_UndefinedBehaviorCatalog`.

  ```c
  /* BUG (illustration only, not compiled): unsigned i can never go below 0, */
  /* so "i >= 0" is always true and i-- wraps to UINT_MAX instead of stopping. */
  /*
   * for (unsigned int i = 10; i >= 0; i--)
   * {
   *     printf("%u\n", i);
   * }
   */

  /* FIX: loop on a signed/inclusive bound, or add 1 before the comparison so */
  /* the unsigned index never has to be compared against a negative value.   */
  unsigned int i;

  for (i = 10; i < 11; i--)
  {
      printf("%u\n", i);
      if (i == 0)
      {
          break;
      }
  }
  /* Verified with gcc -std=c99 -Wall -Wextra: prints 10 down to 0 and stops. */
  ```
- **A function has multiple `return` statements and `malloc`s a buffer near the top -
  one early return skips the matching `free`.** Classic resource leak on one code path;
  a strong answer proposes a single cleanup exit point or goto-based cleanup. See
  `15_DynamicMemory`.
- **`char buf[16]; strcpy(buf, userInput);`** No bounds checking - `strcpy` will happily
  overflow `buf` if `userInput` is longer than 15 chars. Expect `strncpy`/`snprintf`
  or an explicit length check as the fix. See `65_SecurityDemos`, `07_Strings`.
- **`int n = a * b; char *p = malloc(n);` where `a` and `b` are attacker-influenced.**
  The multiplication can overflow before the `malloc` call ever sees it, allocating a
  buffer far smaller than intended - a classic integer-overflow-to-heap-overflow chain.
  See `65_SecurityDemos`, `57_UndefinedBehaviorCatalog`.

  ```c
  /* BUG (illustration only, not compiled): count * size can overflow size_t */
  /* before malloc ever sees it, so the allocation ends up far too small.   */
  /*
   * char *buf = malloc(count * size);
   */

  /* FIX: check for overflow before multiplying, or use calloc(), which */
  /* performs this same overflow check internally and fails safely.    */
  char *allocFixed(size_t count, size_t size)
  {
      if ((size != 0U) && (count > (SIZE_MAX / size)))
      {
          return NULL;
      }
      return malloc(count * size);
  }
  /* calloc(count, size) is the simpler fix - it does this same check inside */
  /* libc and returns NULL on overflow instead of a truncated allocation.    */
  /* Verified with gcc -std=c99 -Wall -Wextra. */
  ```
- **`float f = 3.14f; int i = *(int*)&f;`** Type-punning through an incompatible pointer
  type violates strict aliasing and is UB, even though it "usually" produces the bit
  pattern you expect. The fix is `memcpy` or a union. See `62_StrictAliasing`.
- **A `switch` on an enum has no `default` case, and a new enum value gets added later.**
  Silent fall-through to nothing rather than a caught error; MISRA-style guidance requires
  a `default` for exactly this reason. See `57_UndefinedBehaviorCatalog` (MISRA section),
  `09_Enum`.

## 3. Design / tradeoff questions

- **When would you choose a linked list over a dynamic array, and vice versa?** Arrays
  win on cache locality and O(1) random access; linked lists win on O(1) insert/delete
  at a known node and no reallocation/copy cost. See `04_array`, `33_LinkedList`.

  ```c
  /* Linked list: O(1) insert at a known node, no shifting or copying. */
  Node *insertFront(Node *head, int value)
  {
      Node *node;

      node = malloc(sizeof(*node));
      node->value = value;
      node->next = head;
      return node;
  }

  /* Dynamic array: O(1) amortized append, but insert-at-front is O(n) */
  /* because every existing element must shift right.                 */
  void insertFrontArray(int *arr, int *len, int *cap, int value)
  {
      int i;

      for (i = *len; i > 0; i--)
      {
          arr[i] = arr[i - 1];
      }
      arr[0] = value;
      (*len)++;
  }
  /* Verified with gcc -std=c99 -Wall -Wextra: both report front value 2. */
  ```
- **Why might you prefer an iterative implementation over a recursive one on an
  embedded target?** Bounded, predictable stack usage matters more than elegance when RAM
  is tiny and there's no guard page to catch overflow. See `14_Recursion`,
  `57_UndefinedBehaviorCatalog` (MISRA section).
- **Hash table with chaining vs open addressing - when would you pick each?** Chaining
  tolerates a high load factor and simplifies deletion; open addressing (with tombstones)
  avoids per-node allocation overhead and is more cache-friendly at low-to-moderate load.
  See `34_HashTable`, `36_AdvancedHashingAndCacheAwareStructures`.

  ```c
  /* Chaining: each slot is a list head, so collisions just grow the list. */
  /* Tolerates a high load factor and deletion never disturbs other keys. */
  void chainInsert(ChainNode *table[], int key)
  {
      int slot;
      ChainNode *node;

      slot = key % TABLE_SIZE;
      node = malloc(sizeof(*node));
      node->key = key;
      node->next = table[slot];
      table[slot] = node;
  }

  /* Open addressing: on collision, probe forward until an empty slot is */
  /* found. No per-node allocation, better cache locality, but degrades  */
  /* fast as the load factor climbs.                                     */
  void openInsert(int table[], int used[], int key)
  {
      int slot;
      int i;

      slot = key % TABLE_SIZE;
      for (i = 0; i < TABLE_SIZE; i++)
      {
          int probe;

          probe = (slot + i) % TABLE_SIZE;
          if (!used[probe])
          {
              table[probe] = key;
              used[probe] = 1;
              return;
          }
      }
  }
  /* Verified with gcc -std=c99 -Wall -Wextra: inserting 3 then 11 (same */
  /* slot 3) shows chaining links both under slot 3, open addressing    */
  /* probes 11 forward into slot 4.                                     */
  ```
- **When is a B-tree preferred over a balanced BST like a red-black tree?** B-trees are
  tuned for block-oriented storage (disk/SSD pages) where minimizing tree height reduces
  I/O, not just comparisons - the reason databases and filesystems use them. See
  `40_BTreeAndBPlusTree`, `39_SelfBalancingTrees`.
- **Why use an LRU cache instead of just a hash table for a cache with limited size?**
  A hash table alone gives O(1) lookup but no eviction ordering; combining it with a
  doubly-linked list gives O(1) lookup and O(1) "least recently used" eviction. See
  `37_LRUCache`.
- **Why choose a skip list over a balanced BST in some concurrent-friendly designs?**
  Skip lists have simpler, more localized rebalancing (just relinking), which makes
  lock-free/fine-grained-locking variants easier to reason about than tree rotations.
  See `51_SkipList`.
- **AVL vs red-black tree - what's the actual tradeoff?** AVL is more rigidly balanced
  (tighter height bound) giving faster lookups; red-black does fewer rotations on
  insert/delete, favoring write-heavy workloads. See `39_SelfBalancingTrees`.
- **When would a Bloom filter be the right tool instead of an exact set?** When you can
  tolerate false positives (never false negatives) in exchange for huge space savings,
  e.g. a pre-check before an expensive disk/network lookup. See
  `35_ProbabilisticDataStructures`.

## 4. Embedded-specific practical questions

- **How would you reduce firmware's memory footprint on a target with very limited
  RAM?** Talk about static/stack allocation over heap, packing structs deliberately,
  reusing buffers, and picking data structures with lower per-element overhead. See
  `59_MemoryAlignmentAndPadding`, `53_FixedPointArithmetic`.

  ```c
  /* Naive: four flags, each stealing a full byte with no bit sharing. */
  typedef struct
  {
      unsigned char flagA;
      unsigned char flagB;
      unsigned char flagC;
      unsigned char flagD;
  } FlagsNaive;

  /* Packed: bit-fields put all four flags in a single byte. */
  typedef struct
  {
      unsigned char flagA : 1;
      unsigned char flagB : 1;
      unsigned char flagC : 1;
      unsigned char flagD : 1;
  } FlagsPacked;
  /* Verified with gcc -std=c99 -Wall -Wextra: sizeof(FlagsNaive) = 4 bytes, */
  /* sizeof(FlagsPacked) = 1 byte. See `27_BitFields`,                      */
  /* `59_MemoryAlignmentAndPadding` for the full padding/alignment picture. */
  ```
- **Why avoid dynamic allocation after initialization in a safety-critical embedded
  system?** Fragmentation and allocation failure become unpredictable over long uptimes;
  static/pool allocation keeps worst-case memory behavior analyzable ahead of time. See
  `57_UndefinedBehaviorCatalog` (MISRA section), `60_AlignedMallocFree`.
- **Explain the difference between a hard fault and a normal exception on an ARM
  Cortex-M, at a conceptual level.** Honest note: this is architecture trivia beyond what
  this repo demonstrates in code - it's fair interview territory but there's no folder
  here to point to; expect it if the role is Cortex-M-specific.
- **How would you debug a system that only fails intermittently in the field, where you
  can't attach a debugger?** Discuss logging strategy, watchdog/reset-reason capture,
  post-mortem crash dumps, and reproducing under a sanitizer/stress test locally first.
  See `67_GdbWorkflow`, `68_ValgrindAndAsan`, `70_CombinedDebuggingCaseStudy`.
- **Why would you use fixed-point arithmetic instead of floating point on some embedded
  targets?** No FPU means float ops are emulated in software (slow, non-deterministic
  timing); fixed-point gives deterministic, cheap integer-only arithmetic at the cost of
  range/precision tradeoffs you must manage explicitly. See `53_FixedPointArithmetic`.
- **How would you implement a lock-free (or ISR-safe) producer/consumer buffer between
  an interrupt handler and main-loop code?** A single-producer/single-consumer ring
  buffer with volatile head/tail indices avoids needing a full mutex inside an ISR. See
  `32_StackAndQueue` (ring buffer example), `16_ConstVolatile`.
- **Why is a checksum/CRC used on data read over a bus (I2C/SPI/UART) instead of just
  trusting the bytes?** Physical links introduce bit errors; a CRC gives cheap, strong
  detection without needing full error-correction hardware. See `05_BitManipulation`
  (CRC-8 example).
- **What does `volatile` actually do, and why is it not a substitute for a mutex?**
  It only prevents the compiler from caching/reordering that specific variable's memory
  accesses - it says nothing about atomicity or memory ordering between multiple
  variables/threads. See `16_ConstVolatile`.

## 5. Explain-it-simply questions

- **Explain what happens when you call `malloc`, in as much detail as you can.** A
  strong answer covers the allocator finding/splitting a free block (or asking the OS via
  `brk`/`mmap` for more), returning a pointer to usable memory, and why the memory's
  contents are unspecified. See `15_DynamicMemory`, `59_MemoryAlignmentAndPadding`.

  ```c
  /* Success: allocator finds/splits a free block and returns usable memory. */
  /* Contents are unspecified until you write to it - never assume zeros.   */
  okBuf = malloc(BUFFER_LEN * sizeof(*okBuf));
  if (okBuf == NULL)
  {
      return 1;
  }
  okBuf[0] = 42;
  free(okBuf);

  /* Failure: an unreasonably large request returns NULL instead of memory. */
  /* Always check before dereferencing - a missed check is a NULL deref.    */
  hugeSize = (size_t)-1;
  hugeBuf = malloc(hugeSize);
  if (hugeBuf == NULL)
  {
      printf("allocation failed as expected\n");
  }
  /* Verified with gcc -std=c99 -Wall -Wextra: prints "okBuf[0] = 42" then */
  /* "allocation failed as expected".                                     */
  ```
- **Explain the difference between a compiler warning and an error, and why some teams
  treat warnings as errors.** A warning is code that compiles but looks suspicious
  (implicit conversion, unused variable, possible UB); promoting warnings to errors
  (`-Werror`) forces teams to resolve or explicitly silence them rather than drift. See
  `57_UndefinedBehaviorCatalog`, `29_CompilerPipelineWalkthrough`.
- **Explain why `const` correctness matters, to someone unfamiliar with C.** It's a
  compile-time promise about what a function will and won't modify, letting callers reason
  about aliasing/side effects without reading the implementation, and catching accidental
  writes at compile time. See `16_ConstVolatile`.
- **Explain the difference between stack and heap memory, simply.** Stack is
  automatic, fast, and scoped to a function call; heap is manually managed, longer-lived,
  and requires explicit `free`. See `15_DynamicMemory`, `03_pointers`.
- **Explain what a memory leak is to a non-C programmer.** Memory that was allocated but
  never released and is no longer reachable by the program - it isn't a "crash," it's
  wasted memory that accumulates until something else fails. See `15_DynamicMemory`,
  `68_ValgrindAndAsan`.
- **Explain the difference between a shallow copy and a deep copy.** A shallow copy
  duplicates a struct's fields (including any pointers) without duplicating what those
  pointers point to; a deep copy also duplicates the pointed-to data so the two copies
  are fully independent. See `08_Structures`, `16_ConstVolatile`.

  ```c
  /* Shallow copy: duplicates the struct, but both copies share one buffer. */
  Person shallowCopy(Person src)
  {
      return src;
  }

  /* Deep copy: duplicates the pointed-to data too, so copies are independent. */
  Person deepCopy(Person src)
  {
      Person copy;

      copy.name = malloc(strlen(src.name) + 1);
      strcpy(copy.name, src.name);
      return copy;
  }
  /* Verified with gcc -std=c99 -Wall -Wextra: after mutating original.name  */
  /* to "Bob", shallow.name reads "Bob" (aliased) while deep.name still     */
  /* reads "Alice" (independent).                                          */
  ```
- **Explain what a race condition is, without using the word "thread."** Two pieces of
  work whose outcome depends on the unpredictable order/timing in which they touch shared
  state - the same input can produce different results run to run. See
  `66_ThreadSanitizerDemo`.
- **Explain why C doesn't have exceptions, and how error handling is typically done
  instead.** C favors explicit return-code/errno-style error propagation that the caller
  must check, trading convenience for predictable control flow and no hidden unwinding
  cost. See `02_Functions`, `57_UndefinedBehaviorCatalog`.
