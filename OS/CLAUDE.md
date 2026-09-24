# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository purpose

This is a personal OS-internals learning/practice repository — a collection of small,
standalone C programs and notes exploring operating-system concepts: process management
(process control blocks/states, lifecycle, resource limits, signals, errno, system
calls), threading models and synchronization (classical sync algorithms, race
conditions, mutexes/semaphores/monitors, priority inversion, deadlock detection/
avoidance including the Resource Allocation Graph and Banker's Algorithm, starvation/
livelock, atomics and lock-free structures), scheduling internals (scheduling concepts,
process/CPU scheduling, context switch mechanics), comprehensive memory management
(addressing and fragmentation, paging, segmentation, virtual memory, swap/thrashing/
working set, mmap, copy-on-write, false sharing, cache coherence/NUMA/hardware buses,
custom allocators, kernel memory allocators and virtualization), IPC and networking
(pipes, SysV IPC, POSIX queues, the network stack, sockets, I/O multiplexing, RPC),
filesystem internals (structures/allocation, filesystem internals, disk scheduling
algorithms, spooling/buffering/free space management), the boot process, and dynamic
linking/loading and static/shared libraries. It was split out of the sibling
`../C_Basics/` repo, which is meant for core C-language/tooling topics rather than OS
internals. There is no test suite or package manifest. `code/` has a Makefile for bulk
builds; each `.c` file is otherwise self-contained and independently compilable.

## Structure

- `code/` — numbered topic folders (`01_BootProcess` through
  `61_StaticAndSharedLibraries`) in easiest-to-hardest order, each holding small example
  programs for that topic: `01_BootProcess`, `02_Processes`,
  `03_ProcessControlBlockAndStates`, `04_ProcessLifecycle`, `05_ResourceLimits`,
  `06_SignalHandling`, `07_ErrnoAndErrorHandling`, `08_SystemCalls`, `09_Threads`,
  `10_ThreadingModels`, `11_VolatileVsAtomicEmbedded`, `12_RaceConditionAndCriticalSection`,
  `13_ClassicalSyncAlgorithms`, `14_SyncProblems`, `15_MutexVsSemaphoreAndMonitors`,
  `16_PriorityInversion`, `17_DeadlockDetectionAvoidance`,
  `18_ResourceAllocationGraphAndBankersAlgorithm`,
  `19_StarvationLivelockAndDeadlockPrevention`, `20_Atomics`, `21_AdvancedSyncPrimitives`,
  `22_SchedulingConceptsDeepDive`, `23_ProcessScheduling`, `24_CPUScheduling`,
  `25_AdvancedSchedulingAlgorithms`, `26_ContextSwitchMechanics`,
  `27_MemoryAddressingAndFragmentation`, `28_MemoryManagement`, `29_Paging`,
  `30_PageTableEntriesAndTLB`, `31_SegmentationAndOverlays`, `32_VirtualMemoryDeepDive`,
  `33_SwapThrashingAndWorkingSet`, `34_MmapFile`,
  `35_CopyOnWrite`, `36_LockFreeRingBuffer`, `37_ConcurrentDataStructures`,
  `38_FalseSharing`, `39_CacheCoherenceMESI`,
  `40_NUMABasics`, `41_HardwareBuses`, `42_CustomAllocator`,
  `43_KernelMemoryAllocatorsAndVirtualization`, `44_HypervisorsAndVirtualMachines`,
  `45_IPC`, `46_SystemVIPC`,
  `47_NamedPipesAndPosixQueues`, `48_NetworkStackBasics`, `49_SocketProgramming`,
  `50_UDPSockets`, `51_IOMultiplexing`, `52_ZeroCopyIOAndIoUring`, `53_RpcMechanisms`,
  `54_FileSystemStructuresAndAllocation`, `55_FilesystemInternals`,
  `56_DiskSchedulingAlgorithms`, `57_SpoolingBufferingAndFreeSpace`,
  `58_IOManagementPollingInterruptsDMA`,
  `59_LinkerAndLoaderMechanics`, `60_DynamicLoading`, `61_StaticAndSharedLibraries`.
  Some folders are NOTES.md-only (a concise, bullet-point, interview-focused writeup of
  the concept, distinct from a runnable demo) when the topic is more conceptual than
  code, or when a meaningful demo either duplicates an existing folder's code or needs
  hardware/environment this repo can't assume (e.g. multi-socket NUMA hardware or real
  UART/I2C/SPI/PCIe hardware, or a modern kernel/`liburing` this repo's plain-gcc setup
  can't assume): `01_BootProcess`, `40_NUMABasics`, `41_HardwareBuses`,
  `39_CacheCoherenceMESI`, `44_HypervisorsAndVirtualMachines`, `48_NetworkStackBasics`,
  `52_ZeroCopyIOAndIoUring`.
  Other folders pair a runnable demo
  with a `NOTES.md` alongside it, e.g. `07_ErrnoAndErrorHandling`,
  `11_VolatileVsAtomicEmbedded`, `17_DeadlockDetectionAvoidance`,
  `21_AdvancedSyncPrimitives`, `25_AdvancedSchedulingAlgorithms`, `26_ContextSwitchMechanics`,
  `32_VirtualMemoryDeepDive`, `37_ConcurrentDataStructures`,
  `47_NamedPipesAndPosixQueues`, `53_RpcMechanisms`, `55_FilesystemInternals`,
  `58_IOManagementPollingInterruptsDMA`,
  `59_LinkerAndLoaderMechanics`, `61_StaticAndSharedLibraries`.
  `01_BootProcess` covers the computer boot sequence (power-on -> BIOS/UEFI/POST ->
  bootloader -> kernel/init -> services -> login), placed immediately before
  `02_Processes` since the process concept doesn't exist until init/PID 1 starts.
  `47_NamedPipesAndPosixQueues` contrasts a named pipe/FIFO with the anonymous pipe in
  `45_IPC`, and a POSIX message queue with the SysV message queue in `46_SystemVIPC`,
  plus an IPC mechanism comparison table in its `NOTES.md`. `53_RpcMechanisms` is a
  hand-rolled RPC-over-TCP demo built on `49_SocketProgramming`, with `NOTES.md` covering
  marshalling, sync/async RPC, delivery semantics, and real-world frameworks like
  gRPC/Thrift/JSON-RPC/ONC RPC. `21_AdvancedSyncPrimitives` covers `pthread_rwlock_t` and
  `pthread_barrier_t`, complementing `09_Threads`/`14_SyncProblems`, plus (as of the
  `03_fairRwLockFromScratch.c` addition) a from-scratch mutex+condvar reader-writer lock
  built to be explicitly fair/starvation-free, contrasted against
  `pthread_rwlock_t`'s implementation-defined (reader-favoring on glibc) fairness.
  `59_LinkerAndLoaderMechanics` and `60_DynamicLoading` cover linker/loader mechanics and
  `dlopen`/`dlsym`-based dynamic loading; `61_StaticAndSharedLibraries` covers one library
  built both as `.a` and `.so`, with `ldd`/`nm`/`objdump -T` notes.
  `18_ResourceAllocationGraphAndBankersAlgorithm` complements `17_DeadlockDetectionAvoidance`
  with the classic graph-based detection/avoidance model and Banker's Algorithm safety
  checks, and `19_StarvationLivelockAndDeadlockPrevention` rounds out the deadlock/
  liveness topics with prevention strategies distinct from avoidance/detection.
  `25_AdvancedSchedulingAlgorithms` adds Multilevel Queue/Multilevel Feedback Queue
  scheduling plus Rate Monotonic/Earliest Deadline First real-time scheduling on top of
  `22_SchedulingConceptsDeepDive`/`24_CPUScheduling`. `44_HypervisorsAndVirtualMachines`
  covers Type 1/Type 2 hypervisors, full vs paravirtualization, and hardware-assisted
  virtualization (VT-x/EPT), cross-referencing `43_KernelMemoryAllocatorsAndVirtualization`
  (containers) and `30_PageTableEntriesAndTLB` (nested page tables).
  `58_IOManagementPollingInterruptsDMA` covers polling vs interrupt-driven I/O vs DMA,
  device drivers, and single/double/circular buffering, cross-referencing `08_SystemCalls`
  (ioctl) and `57_SpoolingBufferingAndFreeSpace` (buffering vs spooling).
  `36_LockFreeRingBuffer` pairs the original SPSC ring buffer with a Vyukov-style
  lock-free MPMC ring buffer (`02_mpmcRingBuffer.c`) using CAS loops and per-slot
  sequence numbers, contrasted in `NOTES.md` against the SPSC version's CAS-free
  single-owner-index design. `37_ConcurrentDataStructures` covers a lock-striped
  concurrent hash map (`01_lockStripedHashMap.c`) plus a conceptual (NOTES.md-only)
  treatment of concurrent skip lists, cross-referencing `21_AdvancedSyncPrimitives` and
  `36_LockFreeRingBuffer`. `32_VirtualMemoryDeepDive/NOTES.md` includes a numbered
  step-by-step trace of a Linux page fault (MMU failure through PTE/TLB update and
  instruction re-execution), cross-referencing `30_PageTableEntriesAndTLB` and
  `35_CopyOnWrite`. `52_ZeroCopyIOAndIoUring` is a NOTES.md-only writeup of
  `sendfile()`/`splice()`, hugepages (THP vs `MAP_HUGETLB`), and `io_uring`, contrasted
  against `epoll` in `51_IOMultiplexing` and cross-referencing `30_PageTableEntriesAndTLB`.
  Core C-language/tooling topics (loops, pointers, data structures, generics, debugging
  tooling, etc.) remain in the sibling `../C_Basics/` repo (see `../C_Basics/CLAUDE.md`).

## Working with this codebase

- `code/Makefile` builds every `.c`/`.cpp` file under `code/` into a matching binary under
  `code/bin/`, mirroring the source's directory structure (e.g.
  `code/09_Threads/01_pthreadBasics.c` → `code/bin/09_Threads/01_pthreadBasics`).
  From `code/`:
  ```
  make          # build all binaries into bin/
  make clean    # remove bin/
  ```
  A handful of folders need special multi-file or extra-library link rules, documented
  inline in the Makefile: `59_LinkerAndLoaderMechanics` (multi-file: `01_main.c` +
  `dataSections.c`), `60_DynamicLoading` (`plugin.c` built as a shared lib with
  `-fvisibility=hidden`; `01_loader` links with `-ldl`), `61_StaticAndSharedLibraries`
  (`libgreet.c` built into both a static archive and a shared library; `02_useShared`
  links with `-Wl,-rpath,'$ORIGIN'`), and `47_NamedPipesAndPosixQueues/02_posixMessageQueue.c`
  (needs `-lrt` for `mq_*` functions).
- New example files should follow the existing naming pattern within their folder:
  a two-digit numeric prefix followed by a short descriptive name
  (e.g. `01_pthreadBasics.c`, `02_execFamily.c`).
