# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with
code in this repository.

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
  `64_StaticAndSharedLibraries`) in easiest-to-hardest order, each holding small example
  programs for that topic: `01_BootProcess`, `02_Processes`,
  `03_ProcessControlBlockAndStates`, `04_ProcessLifecycle`, `05_ResourceLimits`,
  `06_SignalHandling`, `07_ErrnoAndErrorHandling`, `08_SystemCalls`, `09_Threads`,
  `10_ThreadingModels`, `11_VolatileVsAtomicEmbedded`, `12_RaceConditionAndCriticalSection`,
  `13_ClassicalSyncAlgorithms`, `14_SyncProblems`, `15_MutexVsSemaphoreAndMonitors`,
  `16_PriorityInversion`, `17_DeadlockDetectionAvoidance`,
  `18_ResourceAllocationGraphAndBankersAlgorithm`,
  `19_StarvationLivelockAndDeadlockPrevention`, `20_Atomics`, `21_AdvancedSyncPrimitives`,
  `22_SchedulingConceptsDeepDive`, `23_RTOSConceptsAndTaskScheduling`,
  `24_ProcessScheduling`, `25_CPUScheduling`,
  `26_AdvancedSchedulingAlgorithms`, `27_ContextSwitchMechanics`,
  `28_MemoryAddressingAndFragmentation`, `29_MemoryManagement`, `30_Paging`,
  `31_PageTableEntriesAndTLB`, `32_SegmentationAndOverlays`, `33_VirtualMemoryDeepDive`,
  `34_SwapThrashingAndWorkingSet`, `35_MmapFile`,
  `36_CopyOnWrite`, `37_LockFreeRingBuffer`, `38_ConcurrentDataStructures`,
  `39_FalseSharing`, `40_CacheCoherenceMESI`,
  `41_NUMABasics`, `42_HardwareBuses`, `43_EmbeddedReliabilityAndPowerManagement`,
  `44_FirmwareUpdateAndFlashStorage`,
  `45_CustomAllocator`,
  `46_KernelMemoryAllocatorsAndVirtualization`, `47_HypervisorsAndVirtualMachines`,
  `48_IPC`, `49_SystemVIPC`,
  `50_NamedPipesAndPosixQueues`, `51_NetworkStackBasics`, `52_SocketProgramming`,
  `53_UDPSockets`, `54_IOMultiplexing`, `55_ZeroCopyIOAndIoUring`, `56_RpcMechanisms`,
  `57_FileSystemStructuresAndAllocation`, `58_FilesystemInternals`,
  `59_DiskSchedulingAlgorithms`, `60_SpoolingBufferingAndFreeSpace`,
  `61_IOManagementPollingInterruptsDMA`,
  `62_LinkerAndLoaderMechanics`, `63_DynamicLoading`, `64_StaticAndSharedLibraries`.
  Some folders are NOTES.md-only (a concise, bullet-point, interview-focused writeup of
  the concept, distinct from a runnable demo) when the topic is more conceptual than
  code, or when a meaningful demo either duplicates an existing folder's code or needs
  hardware/environment this repo can't assume (e.g. multi-socket NUMA hardware or real
  UART/I2C/SPI/PCIe hardware, or a modern kernel/`liburing` this repo's plain-gcc setup
  can't assume): `01_BootProcess`, `41_NUMABasics`, `42_HardwareBuses`,
  `40_CacheCoherenceMESI`, `47_HypervisorsAndVirtualMachines`, `51_NetworkStackBasics`,
  `55_ZeroCopyIOAndIoUring`, `23_RTOSConceptsAndTaskScheduling`,
  `43_EmbeddedReliabilityAndPowerManagement`, `44_FirmwareUpdateAndFlashStorage`
  (the latter three for the same reason -
  real RTOS primitives, watchdog/sleep-mode hardware, and OTA/flash hardware can't be
  exercised by a plain userspace C program on a dev box).
  Other folders pair a runnable demo
  with a `NOTES.md` alongside it, e.g. `07_ErrnoAndErrorHandling`,
  `11_VolatileVsAtomicEmbedded`, `17_DeadlockDetectionAvoidance`,
  `21_AdvancedSyncPrimitives`, `26_AdvancedSchedulingAlgorithms`, `27_ContextSwitchMechanics`,
  `33_VirtualMemoryDeepDive`, `38_ConcurrentDataStructures`,
  `50_NamedPipesAndPosixQueues`, `56_RpcMechanisms`, `58_FilesystemInternals`,
  `61_IOManagementPollingInterruptsDMA`,
  `62_LinkerAndLoaderMechanics`, `64_StaticAndSharedLibraries`.
  `01_BootProcess` covers the computer boot sequence (power-on -> BIOS/UEFI/POST ->
  bootloader -> kernel/init -> services -> login), placed immediately before
  `02_Processes` since the process concept doesn't exist until init/PID 1 starts.
  `50_NamedPipesAndPosixQueues` contrasts a named pipe/FIFO with the anonymous pipe in
  `48_IPC`, and a POSIX message queue with the SysV message queue in `49_SystemVIPC`,
  plus an IPC mechanism comparison table in its `NOTES.md`. `56_RpcMechanisms` is a
  hand-rolled RPC-over-TCP demo built on `52_SocketProgramming`, with `NOTES.md` covering
  marshalling, sync/async RPC, delivery semantics, and real-world frameworks like
  gRPC/Thrift/JSON-RPC/ONC RPC. `21_AdvancedSyncPrimitives` covers `pthread_rwlock_t` and
  `pthread_barrier_t`, complementing `09_Threads`/`14_SyncProblems`, plus (as of the
  `03_fairRwLockFromScratch.c` addition) a from-scratch mutex+condvar reader-writer lock
  built to be explicitly fair/starvation-free, contrasted against
  `pthread_rwlock_t`'s implementation-defined (reader-favoring on glibc) fairness.
  `62_LinkerAndLoaderMechanics` and `63_DynamicLoading` cover linker/loader mechanics and
  `dlopen`/`dlsym`-based dynamic loading; `64_StaticAndSharedLibraries` covers one library
  built both as `.a` and `.so`, with `ldd`/`nm`/`objdump -T` notes.
  `18_ResourceAllocationGraphAndBankersAlgorithm` complements `17_DeadlockDetectionAvoidance`
  with the classic graph-based detection/avoidance model and Banker's Algorithm safety
  checks, and `19_StarvationLivelockAndDeadlockPrevention` rounds out the deadlock/
  liveness topics with prevention strategies distinct from avoidance/detection.
  `26_AdvancedSchedulingAlgorithms` adds Multilevel Queue/Multilevel Feedback Queue
  scheduling plus Rate Monotonic/Earliest Deadline First real-time scheduling on top of
  `22_SchedulingConceptsDeepDive`/`25_CPUScheduling`. `47_HypervisorsAndVirtualMachines`
  covers Type 1/Type 2 hypervisors, full vs paravirtualization, and hardware-assisted
  virtualization (VT-x/EPT), cross-referencing `46_KernelMemoryAllocatorsAndVirtualization`
  (containers) and `31_PageTableEntriesAndTLB` (nested page tables).
  `61_IOManagementPollingInterruptsDMA` covers polling vs interrupt-driven I/O vs DMA,
  device drivers, and single/double/circular buffering, cross-referencing `08_SystemCalls`
  (ioctl) and `60_SpoolingBufferingAndFreeSpace` (buffering vs spooling).
  `37_LockFreeRingBuffer` pairs the original SPSC ring buffer with a Vyukov-style
  lock-free MPMC ring buffer (`02_mpmcRingBuffer.c`) using CAS loops and per-slot
  sequence numbers, contrasted in `NOTES.md` against the SPSC version's CAS-free
  single-owner-index design. `38_ConcurrentDataStructures` covers a lock-striped
  concurrent hash map (`01_lockStripedHashMap.c`) plus a conceptual (NOTES.md-only)
  treatment of concurrent skip lists, cross-referencing `21_AdvancedSyncPrimitives` and
  `37_LockFreeRingBuffer`. `33_VirtualMemoryDeepDive/NOTES.md` includes a numbered
  step-by-step trace of a Linux page fault (MMU failure through PTE/TLB update and
  instruction re-execution), cross-referencing `31_PageTableEntriesAndTLB` and
  `36_CopyOnWrite`. `55_ZeroCopyIOAndIoUring` is a NOTES.md-only writeup of
  `sendfile()`/`splice()`, hugepages (THP vs `MAP_HUGETLB`), and `io_uring`, contrasted
  against `epoll` in `54_IOMultiplexing` and cross-referencing `31_PageTableEntriesAndTLB`.
  `23_RTOSConceptsAndTaskScheduling` covers RTOS (FreeRTOS/Zephyr/ThreadX-style)
  concepts distinct from a general-purpose OS scheduler - deterministic bounded-latency
  preemption, task notifications, priority-inheritance mutexes, and per-task stack
  sizing/overflow detection - placed right after `22_SchedulingConceptsDeepDive` since
  it builds on that folder's (and `25_CPUScheduling`'s) real-time scheduling theory
  (RMS/EDF), before the general-purpose `24_ProcessScheduling` demo.
  `42_HardwareBuses/NOTES.md` also covers CAN (Controller Area Network), contrasted
  against UART/I2C/SPI/PCIe on message-based (not address-based) arbitration.
  `43_EmbeddedReliabilityAndPowerManagement` covers watchdog timers (simple vs
  windowed), microcontroller low-power sleep states, and stack-overflow risk on
  memory-constrained embedded targets (including GCC's `-fstack-usage` for static
  stack analysis), placed right after `42_HardwareBuses` as the other
  notes-only embedded-hardware topic, before `44_FirmwareUpdateAndFlashStorage`.
  `44_FirmwareUpdateAndFlashStorage` (NOTES.md-only, for the same real-hardware reason
  as `42_HardwareBuses`/`43_EmbeddedReliabilityAndPowerManagement`) covers dual-bank/
  A-B OTA firmware update design and its power-loss safety, firmware signing/
  verification (cross-referencing `01_BootProcess`'s Secure Boot chain-of-trust), and
  flash erase-before-write constraints and wear leveling, cross-referencing
  `43_EmbeddedReliabilityAndPowerManagement`'s watchdog coverage for the rollback
  trigger; placed right before `45_CustomAllocator`.
  `23_RTOSConceptsAndTaskScheduling/NOTES.md` also covers WCET (Worst-Case Execution
  Time) analysis - static vs measurement-based approaches - as the input
  `26_AdvancedSchedulingAlgorithms`'s RMS/EDF schedulability tests actually need.
  `42_HardwareBuses/NOTES.md` also covers the HAL (chip-family register abstraction)
  vs BSP (specific-board configuration) layering, cross-referencing the memory-mapped
  register access in `../../C_Basics/code/16_ConstVolatile`.
  `62_LinkerAndLoaderMechanics/NOTES.md` also covers embedded linker scripts (`.ld`
  `MEMORY`/`SECTIONS` blocks placing code/data into fixed FLASH/RAM regions) and the
  startup code's `.data`-copy/`.bss`-zero job before `main()`, contrasted with the
  hosted-OS loader/dynamic-linker case, cross-referencing
  `28_MemoryAddressingAndFragmentation`/`29_MemoryManagement`.
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
  inline in the Makefile: `62_LinkerAndLoaderMechanics` (multi-file: `01_main.c` +
  `dataSections.c`), `63_DynamicLoading` (`plugin.c` built as a shared lib with
  `-fvisibility=hidden`; `01_loader` links with `-ldl`), `64_StaticAndSharedLibraries`
  (`libgreet.c` built into both a static archive and a shared library; `02_useShared`
  links with `-Wl,-rpath,'$ORIGIN'`), and `50_NamedPipesAndPosixQueues/02_posixMessageQueue.c`
  (needs `-lrt` for `mq_*` functions).
- New example files should follow the existing naming pattern within their folder:
  a two-digit numeric prefix followed by a short descriptive name
  (e.g. `01_pthreadBasics.c`, `02_execFamily.c`).
