Three OS-facing topics rounding out the process/IPC material in
`Notes/16_SystemCalls.c`.

System V IPC (`<sys/ipc.h>` and friends) - an older, key-based alternative
to the POSIX mechanisms already covered in `code/66_IPC/`:

*   Shared memory - shmget()/shmat()/shmdt()/shmctl(IPC_RMID) manage a
    kernel-owned segment identified by a key, with its own lifetime that
    outlives any single process until explicitly removed - contrast with
    the POSIX `mmap(MAP_SHARED|MAP_ANONYMOUS)` region in
    `66_IPC/02_sharedMemory.c`, which only exists for as long as some
    process keeps it mapped. See
    `code/67_SystemVIPC/01_sharedMemorySegment.c`.
*   Message queues - msgget()/msgsnd()/msgrcv() manage a kernel-owned list
    of discrete, type-tagged messages, letting a receiver selectively pull
    a specific message type out of order - unlike a pipe's undifferentiated
    byte stream (`66_IPC/01_pipeCommunication.c`). See
    `code/67_SystemVIPC/02_messageQueue.c`.
*   Semaphore sets - semget()/semop()/semctl() operate on a *set* of
    semaphores by index (even a "single" semaphore is a 1-element set),
    versus the single POSIX `sem_t` in `66_IPC/03_processSemaphore.c`. See
    `code/67_SystemVIPC/03_semaphoreSet.c`.

Command-line option parsing:

*   getopt() walks argv looking for recognized single-character flags
    (e.g. `-v`, `-o value`), setting `optarg` when a flag takes a value and
    leaving `optind` pointing at the first non-option argument - built on
    top of the same raw argc/argv already covered in
    `16_CommandLineArgs/01_argcArgv.c`, but with the parsing loop written
    for you. See `code/24_CommandLineOptions/01_getoptBasics.c`.

Process scheduling and inspection:

*   A process's "nice" value (-20 to 19, set/adjusted via nice()) is a
    hint to the OS scheduler about relative CPU priority - a higher nice
    value means more willing to yield the CPU to others. It's a hint, not
    a guarantee, and only root can lower it below the default.
*   /proc/self is a live view of this exact running process's kernel
    state - /proc/self/status was already read for RSS in
    `57_Paging/02_demandPaging.c`; the same file also exposes the
    process's current scheduling state (running, sleeping, etc.). See
    `code/53_ProcessScheduling/01_niceAndProcInspection.c`.
