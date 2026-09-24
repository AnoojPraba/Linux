# Threading Models

Notes-only: this is the conceptual model behind what `09_Threads` already
demonstrates with real `pthread_create()`/mutex/condvar/semaphore code -
Linux pthreads (NPTL) is a concrete instance of the one-to-one model
described below.

## User-level threads vs kernel-level threads

- **User-level threads**: managed entirely by a userspace threading
  library; the kernel doesn't know they exist - it only sees one process.
  - Pros: thread creation and context switching are cheap (no syscall, no
    kernel involvement - just saving/restoring a few registers and jumping).
  - Cons: no true parallelism - the kernel schedules the whole process onto
    one CPU at a time, so the library can only time-slice threads onto that
    one core. Worse, if one thread makes a *blocking syscall*, the kernel
    blocks the entire process, stalling every other user-level thread in it,
    unless the library goes out of its way to multiplex blocking calls
    (e.g. wrapping I/O in non-blocking calls internally).
- **Kernel-level threads**: each thread is a schedulable entity the kernel
  itself knows about and schedules independently.
  - Pros: true parallelism on multi-core (different threads really can run
    on different CPUs simultaneously); a blocking syscall in one thread
    only blocks that thread, others keep running.
  - Cons: heavier context switches (full kernel trap/register-save path)
    and per-thread-operation syscall overhead (`pthread_create` under NPTL
    is a `clone()` syscall).

## Multithreading models (how user threads map to kernel threads)

- **Many-to-one**: many user-level threads multiplexed onto a single
  kernel thread by the userspace library.
  - Fast to create/switch (all in userspace), but no real parallelism
    (only one kernel thread to schedule), and one blocking syscall blocks
    every user thread in the process. Rarely used today (old green-threads
    implementations, e.g. early Java green threads, GNU Portable Threads).
- **One-to-one**: each user thread has its own dedicated kernel thread.
  - True parallelism across cores; the kernel scheduler handles each thread
    independently. Higher overhead per thread (each `pthread_create` is a
    real kernel object/syscall). **This is what Linux pthreads (NPTL -
    Native POSIX Thread Library) actually implements** - every
    `pthread_create()` call in `09_Threads` creates a real kernel-scheduled
    task (`clone()` with shared address space), visible as its own entry
    under `/proc/[pid]/task/`.
- **Many-to-many**: M user threads multiplexed onto N kernel threads
  (M >= N), with the library free to move a user thread between kernel
  threads. Gets parallelism *and* cheap creation for the common case, but
  is significantly more complex to implement correctly (the library needs
  its own scheduler cooperating with the kernel's). Go's goroutines
  (M:N over OS threads) are a modern real-world example; classic
  Solaris/old NPTL predecessors also used variants of this.

| Model | Parallelism | Blocking-syscall impact | Overhead | Real example |
|---|---|---|---|---|
| Many-to-one | No | Blocks all threads | Lowest | old green threads |
| One-to-one | Yes | Isolated to that thread | Highest per-thread | Linux NPTL (pthreads) |
| Many-to-many | Yes | Isolated (if library multiplexes right) | Moderate | Go goroutines |

## Process-based vs thread-based multitasking

- **Separate processes**: each has its own address space (own page tables).
  - Pros: isolation - a crash or memory corruption in one process cannot
    directly corrupt another's memory; the kernel enforces the boundary via
    the MMU.
  - Cons: heavier - context switches change the address space (TLB/cache
    effects), and communication requires IPC (pipes, shared memory,
    sockets - see `45_IPC`/`46_SystemVIPC`), which is slower than sharing
    memory directly.
- **Threads within one process**: share the same address space (code, heap,
  globals, open file descriptors), each with its own stack and register
  set.
  - Pros: cheap communication - threads just read/write shared memory
    directly (protected by mutexes/condvars, per `09_Threads`), and thread
    creation/context-switch is cheaper than a full process switch.
  - Cons: no isolation - a bug in one thread (buffer overrun, wild pointer,
    unsynchronized access) can corrupt state used by every other thread in
    the process, and one thread crashing takes down the whole process.

## Cross-references

- `09_Threads` - `pthread_create()`/mutex/condvar/semaphore demos are all
  concrete one-to-one-model kernel threads under the hood (NPTL); this
  folder is the model that explains *why* they behave the way they do
  (real parallelism, independent blocking, per-thread kernel overhead).
- `26_ContextSwitchMechanics` - contrasts the cheap user-level-thread switch
  with the full kernel-level context switch referenced above.
- `45_IPC`/`46_SystemVIPC` - the IPC mechanisms processes need precisely
  because they don't share an address space the way threads do.
