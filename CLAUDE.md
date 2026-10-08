# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository purpose

This is a personal, comprehensive interview-prep repository organized as five semi-independent domains: C fundamentals, C++, OS internals, system design/architecture, and behavioral/leadership skills. The candidate has 15+ years experience and is preparing for senior-level technical and behavioral interviews. Each domain is housed in its own subdirectory with its own CLAUDE.md for domain-specific guidance.

## Structure

- **C_Basics/** — Core C language learning via standalone, self-contained programs: algorithms (search, sort, pattern matching), data structures (linked lists, hash tables, trees, graphs, tries, heaps), bit manipulation, undefined behavior, memory alignment, debugging tools (gdb, valgrind, ASan, TSan), and tooling (compiler pipeline, CMake, cross-compilation). Makefile-driven bulk builds; each `.c` file compiles independently. Runs `01_Loops` through `82_CoroutinesInC` (the `74`-`82` batch covers `container_of`, integer promotions, cleanup patterns, preprocessor/C11 tricks, branch/cache hints, stack frames/ABI, signal safety/TLS, malloc internals, coroutines in C). `Notes/TIMED_DRILLS.md` has timed write-from-memory drills.

- **Cpp/** — Core C++ language learning via standalone programs: classes, templates, STL, design patterns, concurrency, smart pointers, RAII, and advanced topics (template metaprogramming, type erasure, custom allocators, C++20 features, SOLID principles). Organized by topic folder + separate `DesignPatterns/` subdirectory for creational/structural/behavioral patterns. Makefile-driven; each `.cpp` compiles independently.

- **OS/** — Operating-systems internals via standalone programs and notes: process management, threading and synchronization (classical algorithms, deadlock/resource graphs, atomics, lock-free structures), scheduling, memory management (paging, segmentation, virtual memory, mmap, copy-on-write, false sharing, NUMA), IPC and networking (sockets, RPC, I/O multiplexing), filesystems, boot process, and dynamic linking/shared libraries. Makefile-driven; covers `01_BootProcess` through `76_I2cBusProtocol` (adds futex/seqlock, memory-model litmus tests, epoll depth, containers from scratch, TCP/UDP/concurrent-server examples, interrupts/eBPF/perf notes, UART and I2C).

- **SystemDesign/** — System design and architecture interview prep (conceptual, no runnable code): scalability fundamentals, load balancing, caching, database internals, distributed systems theory (CAP, consistency), messaging/event-driven architecture, API protocol tradeoffs (REST/gRPC/GraphQL), networking, rate limiting, security, capacity estimation, resilience patterns, observability, deployment, and end-to-end case studies (URL shortener, chat system, distributed cache, HFT matching engine, concurrent key-value store, memory pool). Covers `01_ScalabilityBasics` through `35_ContainersAndKubernetesBasics` (`29`-`35` add distributed transactions, LSM trees, Kafka/streaming, vector clocks/CRDTs, news-feed and search case studies, Kubernetes).

- **Behavioral/** — Behavioral and leadership interview prep (conceptual, no code): STAR answer format, senior-level stories (technical disagreements, production incidents, mentoring, pushing back, cross-team influence), a worked example answer, interview-round mechanics (format, follow-up, scoring), people leadership (hiring/performance/delegation), estimation/ADRs, and a question bank with a story matrix.

- **customization/** — Configuration files: `bashrc` (git branch in prompt, aliases like `vimf`, `ws`, `cs`), `vimrc` (full C/C++ dev environment: vim-plug, fzf, coc.nvim, cscope+ctags, clang-format, Termdebug), `clang-format` (style config), `profile` (shell initialization). **Note:** Some paths in these files reference `/home/rasp/` and should be updated to use `~` for portability.

## Common development tasks

### Building and running C code
```bash
cd C_Basics/code
make              # build all .c files in code/ → code/bin/
make clean        # remove code/bin/

# Run a specific example
./bin/05_BitManipulation/04_get_bitset

# Compile and run a single file directly
gcc -Wall -Wextra -std=c99 01_Loops/01_simple_loop.c -o /tmp/a.out && /tmp/a.out
```

### Building and running C++ code
```bash
cd Cpp/code
make              # build all .cpp files in code/ → code/bin/
make clean        # remove code/bin/

./bin/03_ClassesAndObjects/01_basic_class

# Compile a single file
g++ -std=c++17 -Wall -Wextra 03_ClassesAndObjects/01_basic_class.cpp -o /tmp/a.out && /tmp/a.out
```

### Building and running OS code
```bash
cd OS/code
make              # build all .c files in code/ → code/bin/
make clean        # remove code/bin/

./bin/09_Threads/01_create_thread
```

### Compiling Notes/ files
```bash
cd C_Basics
gcc Notes/09_VariableScope.c -o /tmp/a.out && /tmp/a.out
```

### Running with debugging tools
```bash
# GDB
gdb ./bin/05_BitManipulation/04_get_bitset

# Valgrind (memory errors)
valgrind --leak-check=full ./bin/01_Loops/01_simple_loop

# AddressSanitizer (compile-time instrumentation)
gcc -Wall -Wextra -std=c99 -fsanitize=address,undefined 01_Loops/01_simple_loop.c -o /tmp/a.out && /tmp/a.out

# ThreadSanitizer
gcc -Wall -Wextra -std=c99 -fsanitize=thread 09_Threads/01_race_condition.c -o /tmp/a.out && /tmp/a.out
```

### Formatting code
```bash
# Using the vimrc binding (from within vim)
# \cf    : format entire file with clang-format
# Ctrl+K : alternative format binding

# From command line
clang-format -style=file:customization/clang-format -i MyFile.c
```

### Code navigation (from vim)
- **Ctrl+]** — Jump to tag definition (ctags)
- **Ctrl+\>g** — Find global definition (cscope)
- **Ctrl+\>s** — Find symbol references
- **Ctrl+\>c** — Find functions calling this function
- **Ctrl+p** — FZF file search
- **Ctrl+b** — FZF buffer list
- **Ctrl+J** — Clear search highlighting
- **Ctrl+l** — Toggle taglist sidebar

### Cscope database
```bash
# Rebuild the cscope database in the current directory
# (mapped to leader+cr in vim)
rm -f cscope.out cscope.in.out cscope.po.out && cscope -b -C -R -q
```

## Cross-cutting topics and references

- **Network stack and RPC internals:** `OS/code/51_NetworkStackBasics`, `OS/code/52_SocketProgramming`, `OS/code/56_RpcMechanisms` cross-reference `Cpp/code/32_RpcMechanismsCpp` and `SystemDesign/topics/10_APIProtocolsRESTvsGRPCvsGraphQL`.
- **Endianness:** `C_Basics/code/06_EndiannessAndByteOrder` (runtime detection, byte-swapping vs `htonl`) cross-references socket code in `OS/`.
- **B-tree / B+ tree indexing:** `C_Basics/code/40_BTreeAndBPlusTree` (NOTES.md-only, conceptual) cross-references `SystemDesign/topics/04_DatabaseIndexingAndQueryOptimization` and filesystem allocation in `OS/code/57_FileSystemStructuresAndAllocation`.
- **Database transactions and consistency:** `SystemDesign/topics/05_ACIDAndTransactionIsolation`, `08_CAPTheoremAndConsistencyModels`, and `06_SQLvsNoSQLTradeoffs`.
- **Memory and cache:** `C_Basics/code/39_FalseSharing`, `OS/code/39_FalseSharing` (cache-line alignment motivation), `OS/code/40_CacheCoherenceMESI`, and `OS/code/41_NUMABasics` all feed into understanding why certain data-structure patterns matter in high-performance systems.
- **Concurrency primitives:** `OS/code/13_ClassicalSyncAlgorithms` through `21_AdvancedSyncPrimitives` are foundational for `Cpp/code/24_Concurrency`, `OS/code/37_LockFreeRingBuffer`, and `SystemDesign/topics/26_DesignCaseStudyConcurrentInMemoryKeyValueStore`.
- **Behavioral stories for senior interviews:** `Behavioral/topics/01_BehavioralAndLeadershipInterviewPrep` cross-references technical examples from the `C_Basics/`, `Cpp/`, `OS/`, and `SystemDesign/` domains.

## File naming and structure conventions

- **Code files:** Two-digit numeric prefix + descriptive name (e.g., `04_array.c`, `01_simple_loop.c`, `03_copyConstructor.cpp`).
- **NOTES.md files:** Concise, bullet-point, interview-focused writeups — not essay prose. Live in topic folders when a concept is too conceptual for runnable code, or when a meaningful demo would duplicate an existing folder's work.
- **Headers in C_Basics:** `code/05_BitManipulation/utils.h` is shared within that folder only; most topic folders do not share headers. Each file is otherwise fully self-contained.

## Subdirectory-specific guidance

For deep guidance on working within each domain, consult the subdirectory's own CLAUDE.md:
- **C_Basics/CLAUDE.md** — Details on the C curriculum, Makefile, and Notes/ folder.
- **Cpp/CLAUDE.md** — C++ curriculum, design patterns organization, Makefile.
- **OS/CLAUDE.md** — OS internals curriculum, NOTES.md-only topics, and how topics relate to the broader curriculum.
- **SystemDesign/CLAUDE.md** — System-design case studies, conceptual topics, and cross-curriculum references.
- **Behavioral/CLAUDE.md** — Leadership narrative structure and interview-prep strategy for senior candidates.

## Notes

- **No root-level Makefile:** Each domain (C_Basics/, Cpp/, OS/) has its own `code/Makefile`. SystemDesign/ and Behavioral/ are conceptual-only and have no build system.
- **Vim setup:** The `customization/vimrc` is comprehensive for C/C++ dev (vim-plug, coc.nvim, fzf, cscope, clang-format). The configuration references this repository's customization folder and assumes `~/.local/bin/`, `~/.vim/plugged`, etc. are available.
- **Path issues in config files:** `customization/vimrc` and `customization/bashrc` contain hardcoded `/home/rasp/` paths for clang-format; update these to use `~` for portability.
- **Reusable patterns across domains:** Concurrency patterns, memory management techniques, and algorithmic ideas implemented in C_Basics/ and Cpp/ often have corresponding OS-internals or system-design implications explored in the OS/ and SystemDesign/ domains.
