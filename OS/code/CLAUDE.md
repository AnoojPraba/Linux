# OS/code

Index of the 76 numbered OS-internals topic folders (`01_BootProcess` to `76_I2cBusProtocol`), ordered roughly easiest to hardest. Each folder has its own CLAUDE.md with files, build line, concepts, gotchas and cross-references.

## Build everything
- `make` here builds every `.c` into `../bin/<NN_Topic>/<name>` (that is `OS/bin/`, git-ignored), in parallel (`MAKEFLAGS += -j$(nproc)`). `make clean` removes `../bin`.
- Flags: `gcc -Wall -I. -pthread ... -lm` (no `-std` or `-O`).
- Special rules: `62_LinkerAndLoaderMechanics` (`01_main.c` + `dataSections.c`), `63_DynamicLoading` (builds `libplugin.so` with `-fPIC -fvisibility=hidden -shared`, loader linked with `-ldl`; run the loader from its bin dir), `64_StaticAndSharedLibraries` (`libgreet.a` via `ar`, `libgreet.so` with rpath `$ORIGIN`). `dataSections.c`, `plugin.c`, `libgreet.c` have no `main()` and are excluded from the generic rule.
- `-lrt` is not passed anywhere; on this glibc (2.36) POSIX message queues (`50`) link without it.
- Single file by hand: `gcc -Wall -Wextra -std=gnu11 -pthread NN_Topic/file.c -o /tmp/x && /tmp/x` (add `-lm`, `-ldl`, `-lrt` only if the linker asks).

## Numbering and layout
- Two-digit prefix gives the study order, contiguous from `01` to `76`; files inside a folder carry `NN_` prefixes.
- NOTES-only folders (nothing to compile): `01`, `03`, `10`, `15`, `19`, `22`, `23`, `32`, `40`, `41`, `42`, `44`, `46`, `47`, `51`, `55`, `57`, `60`, `70`, `71`.
- Folders `20`, `37`, `54`, `65`-`76` end NOTES.md with a "Senior interviewer Q&A" section.
- Deliberately flawed demos: `12` (data race), `16` (illustrative priority inversion), `17/01` (hangs by deadlock), `08/05` (catches its own SIGSEGV).
- Network demos use loopback only: fixed ports in `52` (8080), `53` (8081), `56` (8090); ephemeral ports (port 0) in `67`, `69`, `73`, `74`. Hardware demos (`75/02`, `76/02`) need real devices.
- Several demos print timings or counts specific to this aarch64 Raspberry Pi (16 KB pages, weak memory ordering).
- `../INTERVIEW_QUESTIONS.md` is the question bank for this domain (one level up from this folder).

## Folder map
| Folder | Purpose |
|---|---|
| `01_BootProcess` | Conceptual walkthrough of what happens from power-on to a login prompt (NOTES-only) |
| `02_Processes` | Process creation basics: fork() duplication and exec* replacing the process image |
| `03_ProcessControlBlockAndStates` | Conceptual notes on the process control block, process table and the process state diagram (NOTES-only) |
| `04_ProcessLifecycle` | Zombie, orphan and daemon processes: what happens after fork when parent and child exit in different orders |
| `05_ResourceLimits` | Per-process resource limits with getrlimit/setrlimit, shown by exhausting file descriptors |
| `06_SignalHandling` | Basic POSIX signal handling: installing a handler, raise() and alarm()/pause() |
| `07_ErrnoAndErrorHandling` | Correct errno usage: only inspect it after a call has reported failure |
| `08_SystemCalls` | The syscall boundary: libc wrapper vs raw syscall, errno/perror, file-system metadata calls, ioctl and mprotect |
| `09_Threads` | POSIX threads: creation, mutexes, condition variables, counting semaphores, lock ordering and an odd/even printing... |
| `10_ThreadingModels` | Conceptual notes on user-level vs kernel-level threads and many-to-one, one-to-one, many-to-many mapping (NOTES-only) |
| `11_VolatileVsAtomicEmbedded` | Why volatile is not atomic: its correct narrow use (signal/ISR flags, MMIO) vs _Atomic for cross-thread sharing |
| `12_RaceConditionAndCriticalSection` | Race conditions and critical sections demonstrated with an unprotected shared counter |
| `13_ClassicalSyncAlgorithms` | Classical mutual-exclusion algorithms (Peterson, Dekker, Bakery) and the hardware test-and-set / compare-and-swap that... |
| `14_SyncProblems` | Classic synchronisation problems solved with pthreads: bounded-buffer producer/consumer, readers-writers and dining... |
| `15_MutexVsSemaphoreAndMonitors` | Conceptual comparison of mutexes, semaphores and monitors (NOTES-only) |
| `16_PriorityInversion` | Priority inversion (the Mars Pathfinder bug) with an illustrative three-thread demo and notes on priority... |
| `17_DeadlockDetectionAvoidance` | A deliberately deadlocking AB/BA lock program and the lock-ordering fix |
| `18_ResourceAllocationGraphAndBankersAlgorithm` | Resource allocation graphs and the Banker's deadlock-avoidance algorithm (safety check and request handling) |
| `19_StarvationLivelockAndDeadlockPrevention` | Conceptual notes on starvation, livelock and the four ways to prevent deadlock (NOTES-only) |
| `20_Atomics` | C11 stdatomic and thread-local storage: atomic counters, memory ordering, patterns, pitfalls |
| `21_AdvancedSyncPrimitives` | Reader/writer locks, barriers and a from-scratch fair (starvation-free) rwlock |
| `22_SchedulingConceptsDeepDive` | Conceptual scheduling vocabulary: long/medium/short-term schedulers, dispatcher vs scheduler, preemption, starvation... |
| `23_RTOSConceptsAndTaskScheduling` | Conceptual notes on RTOS design: tasks, deterministic scheduling, RTOS primitives, per-task stack sizing and WCET... |
| `24_ProcessScheduling` | Process priority in practice: nice values and reading scheduler state from /proc |
| `25_CPUScheduling` | Basic CPU scheduling algorithms simulated on a process set: FCFS, SJF, Round Robin, priority |
| `26_AdvancedSchedulingAlgorithms` | Multilevel queue and feedback-queue schedulers plus real-time scheduling (rate-monotonic, earliest-deadline-first) with... |
| `27_ContextSwitchMechanics` | What a context switch saves and costs |
| `28_MemoryAddressingAndFragmentation` | Contiguous allocation strategies (next fit) and the buddy system |
| `29_MemoryManagement` | Classic memory-management simulations: FIFO vs LRU page replacement and first-fit vs best-fit contiguous allocation |
| `30_Paging` | Paging basics seen from user space: querying the page size, anonymous mmap and observing RSS growth under demand paging |
| `31_PageTableEntriesAndTLB` | Address translation: splitting virtual addresses into page number and offset, PTE fields, multi-level page tables and... |
| `32_SegmentationAndOverlays` | Conceptual notes on segmentation, segmentation vs paging, hybrid schemes and overlays (NOTES-only) |
| `33_VirtualMemoryDeepDive` | Virtual memory and the Linux page-fault path |
| `34_SwapThrashingAndWorkingSet` | Page replacement beyond FIFO/LRU: Optimal, Clock (second chance) and Belady's anomaly |
| `35_MmapFile` | File-backed shared mmap: read and modify a file through memory and flush with msync |
| `36_CopyOnWrite` | Copy-on-write after fork(): parent and child share physical pages until one writes |
| `37_LockFreeRingBuffer` | Bounded lock-free queues: an SPSC ring buffer with plain atomics and a Vyukov-style MPMC queue |
| `38_ConcurrentDataStructures` | Lock-striped concurrent hash map: splitting one global lock into per-stripe locks to reduce contention |
| `39_FalseSharing` | False sharing benchmark: independent per-thread counters in one cache line vs padded to separate lines |
| `40_CacheCoherenceMESI` | Conceptual notes on the MESI cache coherence protocol and why it matters for concurrent code (NOTES-only) |
| `41_NUMABasics` | Conceptual notes on NUMA: local vs remote memory access, node affinity, first-touch policy and libnuma (NOTES-only) |
| `42_HardwareBuses` | Conceptual comparison of UART, I2C, SPI, PCIe and CAN buses plus HAL/BSP (NOTES-only |
| `43_EmbeddedReliabilityAndPowerManagement` | Embedded reliability and power topics (watchdogs, sleep modes, stack limits, debouncing) with a switch-debounce... |
| `44_FirmwareUpdateAndFlashStorage` | Conceptual notes on OTA firmware updates (A/B banks), signing/verification and flash constraints (NOTES-only) |
| `45_CustomAllocator` | Embedded-style custom allocators: arena (bump) and fixed-size pool with an intrusive free list |
| `46_KernelMemoryAllocatorsAndVirtualization` | Conceptual notes on kernel allocators (buddy and slab), memory interleaving |
| `47_HypervisorsAndVirtualMachines` | Conceptual notes on Type 1/Type 2 hypervisors, full vs paravirtualization and hardware-assisted virtualization... |
| `48_IPC` | Core POSIX IPC between related processes and threads: pipes, anonymous shared memory, process-shared semaphores and an... |
| `49_SystemVIPC` | System V IPC: shared memory segment, message queue and semaphore set, each created with a fixed key and removed... |
| `50_NamedPipesAndPosixQueues` | Named pipes (FIFOs) and POSIX message queues |
| `51_NetworkStackBasics` | Conceptual notes on the TCP handshake and teardown, congestion control and epoll vs select/poll internals (NOTES-only) |
| `52_SocketProgramming` | Minimal TCP client and server over loopback on port 8080 |
| `53_UDPSockets` | Minimal UDP datagram server and client over loopback on port 8081 |
| `54_IOMultiplexing` | select, poll and epoll over pipes |
| `55_ZeroCopyIOAndIoUring` | Conceptual notes on zero-copy I/O (sendfile, splice), hugepages and io_uring (NOTES-only) |
| `56_RpcMechanisms` | Hand-rolled line-based RPC over TCP loopback (marshalling, dispatch, client stub) |
| `57_FileSystemStructuresAndAllocation` | Conceptual notes on directory structures, path names, file allocation methods and access methods (NOTES-only) |
| `58_FilesystemInternals` | Inodes, hard links and symbolic links observed with stat()/lstat() |
| `59_DiskSchedulingAlgorithms` | Disk-head scheduling algorithms (FCFS, SSTF, SCAN, C-SCAN and relatives) on a request queue |
| `60_SpoolingBufferingAndFreeSpace` | Conceptual notes on buffering, spooling and free-space management (bitmap, linked list, grouping) (NOTES-only) |
| `61_IOManagementPollingInterruptsDMA` | Polling vs interrupt-driven I/O vs DMA, device drivers, buffering and rules for writing an ISR, simulated in user space |
| `62_LinkerAndLoaderMechanics` | Linker and loader mechanics: how globals are split across ELF sections (.data, .bss, .text) and resolved across... |
| `63_DynamicLoading` | Runtime plugin loading with dlopen/dlsym/dlclose from a shared library built with hidden default visibility |
| `64_StaticAndSharedLibraries` | Building and linking the same library statically (.a) and dynamically (.so) |
| `65_FutexAndSeqlock` | Futex-based mutex (Drepper mutex2) and a seqlock: how fast-path user-space synchronisation meets the kernel |
| `66_MemoryModelLitmusTests` | Memory-model litmus tests (message passing, store buffering), the ABA problem with a tagged Treiber stack |
| `67_EpollInDepth` | epoll level- vs edge-triggered behaviour and a single-threaded edge-triggered echo server with a built-in test client |
| `68_ContainersFromScratch` | Containers from first principles: namespaces via clone(2) and cgroup v2 inspection |
| `69_TcpDeepDive` | TCP details interviewers ask about: socket buffer options and backpressure on loopback |
| `70_InterruptPathAndKernelModules` | Conceptual notes on the interrupt path from device to handler, top/bottom halves, latency sources and writing kernel... |
| `71_EbpfAndTracingBasics` | Conceptual notes on Linux tracing and eBPF with bpftrace/perf/ftrace one-liners (NOTES-only) |
| `72_PerformanceDebuggingMethodology` | A "the service is slow" methodology demo: two accidental slowdowns with fixes, timed side by side |
| `73_ConcurrentTcpServers` | Concurrent TCP server models: fork-per-connection and a thread pool with a bounded queue, each self-driven by built-in... |
| `74_UdpPatterns` | UDP patterns beyond the basic echo: datagram boundaries, request/retry reliability and broadcast/multicast |
| `75_UartSerialProgramming` | UART/serial programming on Linux with termios: a pty-based loopback demo, a generic real-port terminal and a framing... |
| `76_I2cBusProtocol` | I2C protocol from the wire up: a bit-level open-drain bus simulation with an EEPROM slave |

## Cross-domain links
- C building blocks: `../../C_Basics/code` (signals/TLS `80`, allocators `81`, coroutines `82`); C++ concurrency and RPC: `../../Cpp/code/24_Concurrency`, `32_RpcMechanismsCpp`.
- System-design context for networking, containers and storage: `../../SystemDesign/topics`.
