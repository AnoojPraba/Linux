# Interview Question Bank (OS Internals)

A companion to `code/` - the kinds of questions interviewers actually ask, not a
restatement of the NOTES.md content. Each entry cross-references the folder(s) to
review for the underlying material. Short answers only: what a strong response
touches on, not a full model answer.

## 1. Scenario-based / debugging questions

- **A multi-threaded program deadlocks intermittently in production but never in
  testing - how do you find it?** Talk about non-deterministic thread interleaving,
  reproducing with thread sanitizer / stress + delays, lock ordering audits, and
  taking a core dump to inspect held/waited-on locks. See
  `17_DeadlockDetectionAvoidance`, `12_RaceConditionAndCriticalSection`.
  The actual fix once a lock-ordering bug is found is to make ordering
  consistent everywhere (excerpt from
  `17_DeadlockDetectionAvoidance/02_deadlockFixed.c` - see that file for the
  full before/after with the deadlocking version):
  ```c
  // Fix #1: consistent lock ordering. Both threads always acquire mutexA
  // before mutexB, which eliminates circular wait entirely - one of the four
  // necessary conditions for deadlock is now impossible.
  pthread_mutex_lock(&mutexA);
  usleep(ACQUIRE_DELAY_USEC);
  pthread_mutex_lock(&mutexB);
  /* ... critical section ... */
  pthread_mutex_unlock(&mutexB);
  pthread_mutex_unlock(&mutexA);
  ```
- **A process's memory usage keeps growing over days - leak, cache, or
  fragmentation?** Distinguish via RSS vs heap-allocator stats, `valgrind`/ASan for
  leaks, checking if usage plateaus (cache) vs grows unbounded, and whether freed
  chunks are being reused (fragmentation). See `29_MemoryManagement`,
  `28_MemoryAddressingAndFragmentation`, `44_CustomAllocator`.
- **Throughput drops under high load even though CPU isn't at 100% - what do you
  investigate?** Context-switch rate, run-queue length, lock contention causing
  threads to block, and I/O wait time - CPU idle-but-slow usually means something
  else is the bottleneck. See `27_ContextSwitchMechanics`, `24_ProcessScheduling`,
  `21_AdvancedSyncPrimitives`.
- **A service works fine until latency to a dependency rises slightly, then it
  falls over completely - what design flaw does this suggest?** No timeouts or
  backpressure - a thread-per-request model with no bound on outstanding work
  saturates all worker threads/connections once latency creeps up. See
  `10_ThreadingModels`, `53_IOMultiplexing`, `55_RpcMechanisms`.
- **`top` shows high `%wa` (I/O wait) on a database server - what's happening and
  what would you check next?** Threads blocked on disk I/O, not the CPU being
  idle for no reason; check disk queue depth, `iostat`, and disk scheduling
  policy. See `58_DiskSchedulingAlgorithms`, `60_IOManagementPollingInterruptsDMA`.
- **A page-fault rate spikes and latency degrades right after a deploy - how do
  you tell a bug from expected behavior?** Distinguish minor faults (normal, e.g.
  first-touch/COW) from major faults (disk-backed, expensive) and check for
  thrashing if working set now exceeds available RAM. See `33_VirtualMemoryDeepDive`,
  `34_SwapThrashingAndWorkingSet`, `36_CopyOnWrite`.
- **Two threads each hold a lock the other wants, but there's no obvious cycle in
  the code - how do you confirm it's a deadlock and not just slowness?** Take
  thread dumps, build a wait-for graph from held/blocked locks, look for a cycle.
  See `18_ResourceAllocationGraphAndBankersAlgorithm`, `17_DeadlockDetectionAvoidance`.
- **A lock-free queue occasionally produces corrupted data under heavy
  contention - what's a likely root cause?** ABA problem or a missing memory
  barrier/incorrect memory ordering on the CAS. See `37_LockFreeRingBuffer`,
  `20_Atomics`.
  Minimal illustration of the ABA problem itself (not the fix - see
  `37_LockFreeRingBuffer/02_mpmcRingBuffer.c` for the per-slot sequence
  numbers that solve it in a real ring buffer):
  ```c
  atomic_int shared = ORIGINAL_VALUE;
  int expected = ORIGINAL_VALUE;

  // Some other thread mutates shared A -> B -> A between our read of
  // "expected" and the CAS below; the CAS still sees ORIGINAL_VALUE and
  // succeeds, even though the value took a detour we never observed.
  atomic_store(&shared, MUTATED_VALUE);
  atomic_store(&shared, ORIGINAL_VALUE);

  atomic_compare_exchange_strong(&shared, &expected, MUTATED_VALUE); // succeeds
  ```

## 2. Design / tradeoff questions

- **When would you choose multiple processes over multiple threads for a
  concurrent workload?** Fault isolation (a crash in one process doesn't take
  down the rest) and security boundaries outweigh threads' cheaper context
  switches and shared-memory convenience when workers are untrusted or
  crash-prone. See `10_ThreadingModels`, `02_Processes`.
- **When is a mutex preferable to a lock-free data structure, and vice versa?**
  Mutexes are simpler and fine under low/moderate contention; lock-free
  structures avoid priority inversion and blocking under high contention or in
  interrupt/real-time contexts, at the cost of much higher implementation
  complexity. See `15_MutexVsSemaphoreAndMonitors`, `37_LockFreeRingBuffer`,
  `38_ConcurrentDataStructures`.
- **Why might you choose epoll over a thread-per-connection model for a
  high-connection-count server?** Thread-per-connection doesn't scale past a few
  thousand connections (stack memory, context-switch overhead); epoll lets one
  or a few threads multiplex many idle-most-of-the-time sockets. See
  `53_IOMultiplexing`, `10_ThreadingModels`, `51_SocketProgramming`.
  Excerpt showing the one-time registration vs. per-call querying (see
  `53_IOMultiplexing/03_epollServer.c` for the full working server):
  ```c
  // epoll_create1 makes one kernel-side object that remembers the watch
  // list between calls - epoll_ctl registers/modifies/removes individual
  // descriptors once, and epoll_wait then just asks "which of the
  // already-registered descriptors are ready" without re-describing the
  // whole set every time, unlike select()/poll() which take the full set
  // as an argument on every single call.
  epollFd = epoll_create1(0);
  event.events = EPOLLIN;
  event.data.fd = pipeA[0];
  epoll_ctl(epollFd, EPOLL_CTL_ADD, pipeA[0], &event);
  numReady = epoll_wait(epollFd, events, MAX_EVENTS, -1);
  ```
- **When would you use shared memory IPC vs. a socket, given they're both on the
  same host?** Shared memory is faster (no copy, no kernel round-trip per
  message) but requires you to build your own synchronization; sockets give you
  built-in framing/synchronization and work transparently if you later split
  across machines. See `47_IPC`, `48_SystemVIPC`, `51_SocketProgramming`.
- **When does RPC actually make sense vs. just calling a function directly?**
  Once the callee is in a different process or on a different machine and you
  need marshalling, network failure handling, and location transparency; not
  worth the overhead for in-process calls. See `55_RpcMechanisms`, `47_IPC`.
- **Why might you pick `mmap` over `read`/`write` for a large file?** Avoids
  extra copies between kernel and user buffers and lets the OS manage paging
  lazily, but can be worse for sequential one-pass access due to page-fault
  overhead. See `35_MmapFile`, `54_ZeroCopyIOAndIoUring`.
- **Multilevel Feedback Queue vs. a single run queue with priorities - why did
  Linux/most general OSes evolve toward something like MLFQ?** Approximates
  "shortest job first" without knowing job length up front, and adapts to
  I/O-bound vs CPU-bound behavior automatically. See
  `26_AdvancedSchedulingAlgorithms`, `22_SchedulingConceptsDeepDive`.
- **Static vs. dynamic linking - what's the tradeoff for a long-running service
  vs. a short-lived CLI tool?** Dynamic linking saves disk/memory via shared
  pages and eases security patching, at the cost of load-time resolution
  overhead and "dependency hell"; static linking gives a reproducible,
  self-contained binary. See `63_StaticAndSharedLibraries`, `61_LinkerAndLoaderMechanics`.

## 3. "Explain it simply" / fundamentals questions

- **Explain what happens, step by step, when you call `fork()`.** Kernel
  duplicates the calling process's address space (COW-mapped, not eagerly
  copied), file descriptor table, and PCB; both processes resume right after
  the call, distinguished only by the return value. See `02_Processes`,
  `04_ProcessLifecycle`, `36_CopyOnWrite`.
  Minimal illustration of the split return value (see `02_Processes` for a
  fuller process-lifecycle demo):
  ```c
  pid_t pid = fork();

  if (pid > 0)
  {
      printf("parent: child pid = %d\n", pid);
  }
  else if (pid == 0)
  {
      printf("child: my pid = %d\n", getpid());
  }
  ```
- **Explain the difference between a process and a thread to someone
  non-technical.** A process is like a separate office with its own supplies
  (memory/resources); threads are like coworkers sharing one office who can
  grab the same stapler (memory) but need to coordinate to avoid collisions.
  See `02_Processes`, `09_Threads`.
- **Explain what a page fault is and why it's not necessarily a bug.** The MMU
  traps because a virtual address has no valid mapping yet; the kernel often
  handles it transparently (first touch of a lazily-allocated page, COW,
  demand-paged file) - only a fault on a truly invalid address (segfault) is
  a bug. See `33_VirtualMemoryDeepDive`, `30_Paging`, `31_PageTableEntriesAndTLB`.
  Excerpt showing minor faults counted via `getrusage()` as demand paging
  backs each touched page (see
  `33_VirtualMemoryDeepDive/01_pageFaultDemo.c` for the full demo):
  ```c
  getrusage(RUSAGE_SELF, &before);
  buffer = malloc(ALLOC_SIZE);

  // Touch exactly one byte per page - enough to force the kernel to back
  // each page with physical memory, without wastefully writing every byte.
  for (i = 0; i < ALLOC_SIZE; i += PAGE_STRIDE)
  {
      buffer[i] = 1;
  }
  getrusage(RUSAGE_SELF, &after); // ru_minflt rose ~ once per page touched
  ```
- **Explain why context switches are expensive.** Saving/restoring register
  state, flushing/refilling the TLB and CPU caches, and losing cache locality
  all cost real cycles beyond just the scheduler's bookkeeping. See
  `27_ContextSwitchMechanics`, `31_PageTableEntriesAndTLB`.
- **Explain the tradeoff between strong and eventual consistency for a
  distributed cache.** Strong consistency guarantees every reader sees the
  latest write but costs coordination latency/availability; eventual
  consistency is fast and always available but readers may briefly see stale
  data - this is the CAP-theorem-adjacent tradeoff covered more deeply in the
  sibling `SystemDesign` repo. See `50_NetworkStackBasics`, `55_RpcMechanisms`.
- **Explain what a system call actually does under the hood.** Traps from user
  mode to kernel mode via a software interrupt/syscall instruction, the kernel
  validates arguments and performs the privileged operation, then returns
  control and the result to user mode. See `08_SystemCalls`, `07_ErrnoAndErrorHandling`.
- **Explain the difference between paging and segmentation.** Paging divides
  memory into fixed-size frames (simple allocation, no external
  fragmentation, but internal fragmentation); segmentation divides memory
  into variable-size logical units (matches program structure, but suffers
  external fragmentation). See `30_Paging`, `32_SegmentationAndOverlays`.
- **Explain what happens during the boot process before any user process
  exists.** Power-on -> BIOS/UEFI POST -> bootloader -> kernel init -> PID 1
  (init/systemd) -> user-space services - the process concept itself doesn't
  exist until PID 1 starts. See `01_BootProcess`, `02_Processes`.

## 4. Real-time / embedded-specific questions

- **Why would a hard real-time system prefer Rate Monotonic Scheduling's
  predictability over a scheduler that maximizes average throughput?** A missed
  deadline is a system failure, not just degraded performance - RMS gives a
  provable schedulability bound (fixed priority by period) that a
  throughput-optimizing scheduler can't guarantee. See
  `23_RTOSConceptsAndTaskScheduling`, `26_AdvancedSchedulingAlgorithms`.
- **Explain priority inversion and why the Mars Pathfinder bug happened.** A
  low-priority task holds a mutex a high-priority task needs, and an
  unrelated medium-priority task preempts the low-priority holder
  indefinitely, starving the high-priority task; fixed via priority
  inheritance. See `16_PriorityInversion`, `23_RTOSConceptsAndTaskScheduling`.
  Excerpt of the low-priority holder side of the scenario (see
  `16_PriorityInversion/01_priorityInversionDemo.c` for the full three-thread
  demo and its notes on why real reproduction needs `SCHED_FIFO`):
  ```c
  // Runs at low priority, holds sharedResourceLock while a medium-priority
  // thread is free to preempt it and run indefinitely, and a high-priority
  // thread blocks waiting on the same lock the whole time.
  pthread_mutex_lock(&sharedResourceLock);
  usleep(LOW_PRIORITY_HOLD_USEC);
  pthread_mutex_unlock(&sharedResourceLock);
  ```
- **Why does an RTOS task typically have a much smaller, fixed stack size
  compared to a desktop process?** Memory-constrained microcontrollers (often
  no MMU/virtual memory) must statically budget every task's worst-case stack
  usage up front - there's no guard-page-triggered growth like a desktop
  process gets. See `43_EmbeddedReliabilityAndPowerManagement`,
  `23_RTOSConceptsAndTaskScheduling`.
- **What's the difference between a hardware watchdog timer and a software
  watchdog, and when would each fail to catch a hang?** A hardware watchdog
  resets via an independent timer circuit even if the CPU/software is fully
  wedged; a software watchdog runs as code and can itself hang or be starved
  by the same fault it's meant to catch. See
  `43_EmbeddedReliabilityAndPowerManagement`.
- **Why is `volatile` not enough for safe access to a hardware register or
  shared flag from an ISR?** `volatile` only stops the compiler from
  caching/reordering the access; it gives no atomicity or memory-ordering
  guarantee across cores, and doesn't prevent a read-modify-write race. See
  `11_VolatileVsAtomicEmbedded`, `20_Atomics`.
  `volatile` is still the right (and sufficient) tool for the narrower case
  of a same-thread signal-handler flag (see
  `11_VolatileVsAtomicEmbedded/01_volatileNotAtomic.c` for the full demo):
  ```c
  // volatile only tells the compiler "do not cache this in a register / do
  // not reorder or eliminate accesses" - it says nothing about atomicity or
  // memory ordering across threads/cores.
  static volatile sig_atomic_t stopRequested = 0;

  void handleSigint(int signum)
  {
      (void)signum;
      stopRequested = 1;
  }
  ```
- **Why would a CAN bus be preferred over UART for automotive ECU
  communication?** Multi-master message-based arbitration with built-in
  priority and error detection, versus UART's simple point-to-point link with
  no arbitration. See `42_HardwareBuses`.
- **What's the point of a bounded, deterministic worst-case execution time
  (WCET) analysis in embedded systems, and why can't you just profile average
  case?** Hard real-time correctness depends on the worst case, not the
  average - an occasional long path (cache miss, interrupt, branch) that's
  rare in profiling can still violate a deadline in the field. See
  `23_RTOSConceptsAndTaskScheduling`, `22_SchedulingConceptsDeepDive`.

## 5. Concurrency / synchronization design questions

- **How would you implement a fair reader-writer lock, and why does the naive
  version risk writer starvation?** A naive rwlock lets any new reader jump the
  queue ahead of a waiting writer since readers don't conflict with each
  other; fairness requires queuing readers behind an already-waiting writer
  (ticket/FIFO ordering). See `21_AdvancedSyncPrimitives` (the from-scratch
  fair rwlock), `15_MutexVsSemaphoreAndMonitors`.
  Excerpt of the fairness check itself (see
  `21_AdvancedSyncPrimitives/03_fairRwLockFromScratch.c` for the full
  mutex+condvar implementation):
  ```c
  // Checking waitingWriters (not just activeWriters) is what prevents a
  // steady stream of readers from starving a writer that is already queued.
  void fairRwLockReadLock(FairRwLock *lock)
  {
      pthread_mutex_lock(&lock->mutex);
      while ((lock->activeWriters > 0) || (lock->waitingWriters > 0))
      {
          pthread_cond_wait(&lock->readersOk, &lock->mutex);
      }
      lock->activeReaders++;
      pthread_mutex_unlock(&lock->mutex);
  }
  ```
- **Explain the ABA problem in lock-free programming and how it's typically
  solved.** A CAS succeeds because a value returned to its original state
  after being changed and changed back, hiding an intermediate mutation;
  solved with tagged/versioned pointers or hazard pointers. See
  `37_LockFreeRingBuffer`, `20_Atomics`.
- **Why is `volatile` alone insufficient for thread-safe shared state in C,
  despite being commonly confused with `atomic`?** `volatile` prevents
  compiler-level caching/reordering of a single access but provides no
  atomicity for read-modify-write and no cross-thread memory-ordering
  guarantee - `_Atomic`/atomic builtins are needed for actual thread safety.
  See `11_VolatileVsAtomicEmbedded`, `20_Atomics`.
  Before/after: a plain `volatile` counter loses increments under
  contention, a C11 `_Atomic` counter does not (excerpt from
  `11_VolatileVsAtomicEmbedded/02_atomicCounter.c` - see that file for the
  full multi-threaded verification):
  ```c
  // _Atomic gives both atomicity (no lost updates from a torn read-modify-
  // write) and a defined memory ordering across threads - what real
  // concurrent access needs, unlike plain volatile.
  atomic_int sharedCounter = 0;

  void *incrementAtomic(void *arg)
  {
      int i;
      (void)arg;
      for (i = 0; i < ITERATIONS; i++)
      {
          atomic_fetch_add(&sharedCounter, 1); // vs. "sharedCounter++" on a
      }                                        // plain volatile int, which
      return NULL;                             // can lose updates
  }
  ```
- **Walk through the dining philosophers or producer-consumer problem and how
  each classic solution avoids deadlock/starvation.** Resource-ordering
  (dining philosophers) or bounded buffer with condition variables/semaphores
  (producer-consumer), and what breaks if the ordering/signaling is wrong.
  See `13_ClassicalSyncAlgorithms`, `14_SyncProblems`.
  Excerpt of Peterson's algorithm's entry protocol, a related classic
  mutual-exclusion solution needing no hardware atomic instruction (see
  `13_ClassicalSyncAlgorithms/01_petersonsAlgorithm.c` for the full two-
  thread demo with the exit protocol and verification):
  ```c
  // flag[i] declares "thread i wants to enter"; turn breaks ties when both
  // want in at once - whichever thread did NOT set turn last gets to wait.
  void enterCriticalSection(int selfId, int otherId)
  {
      atomic_store(&flag[selfId], 1);
      atomic_store(&turn, otherId);
      while ((atomic_load(&flag[otherId])) && (atomic_load(&turn) == otherId))
      {
          // busy-wait
      }
  }
  ```
- **When would you reach for a condition variable instead of just spinning on
  a flag?** Spinning wastes CPU and doesn't scale under contention or on a
  single core; a condition variable lets the waiting thread block and be
  woken only when the state actually changes. See
  `15_MutexVsSemaphoreAndMonitors`, `21_AdvancedSyncPrimitives`.
- **What's false sharing, and how would you detect and fix it?** Two threads
  writing to logically independent variables that happen to sit on the same
  cache line cause needless cache-coherence traffic; fix with padding/
  alignment. See `39_FalseSharing`, `40_CacheCoherenceMESI`.
- **Design a thread-safe bounded queue for a producer-consumer pipeline -
  what primitives do you need and what failure modes must you avoid?** Mutex +
  two condition variables (or a semaphore pair) for full/empty signaling;
  avoid lost wakeups and spurious-wakeup bugs by checking the predicate in a
  loop. See `14_SyncProblems`, `37_LockFreeRingBuffer`, `38_ConcurrentDataStructures`.
