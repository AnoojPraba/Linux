Edge cases in a process's life beyond the straightforward
fork/exec/wait already covered in `45_Processes/`, plus two memory-mapping
topics that extend the paging/mmap material in `57_Paging/`.

Zombie process:

*   A child that has exited, but whose parent hasn't called wait()/
    waitpid() yet, becomes a zombie - the kernel keeps its exit status
    around (visible in `ps` as state Z) purely so the parent can retrieve
    it later. It isn't consuming CPU or most memory, just a slot in the
    process table, but a parent that never reaps its zombies can
    eventually exhaust that table. See
    `code/46_ProcessLifecycle/01_zombieProcess.c`.

Orphan process:

*   A child whose parent exits before it does becomes an orphan and gets
    re-parented (typically to init / PID 1, or the nearest process
    marked as a subreaper) rather than being left with no parent at all -
    `getppid()` inside the child visibly changes once this happens. See
    `code/46_ProcessLifecycle/02_orphanProcess.c`.

Daemon process:

*   The classic double-fork sequence detaches a process entirely from the
    terminal that started it, so it keeps running as a background service
    instead of dying when that terminal closes: fork once (the original
    exits, leaving an orphan per the mechanism above), call setsid() to
    start a new session and shed the controlling terminal, then fork a
    second time so the final process is never itself a session leader
    (preventing it from ever reacquiring a controlling terminal). A daemon
    has no terminal to print to, so it logs to a file instead of stdout.
    See `code/46_ProcessLifecycle/03_daemonProcess.c`.

File-backed memory mapping:

*   `57_Paging/` only covered anonymous mappings (backed by nothing, just
    zero-filled pages). Mapping an actual open file descriptor instead
    makes reads/writes through the mapped pointer act directly on the
    file's contents, with no explicit read()/write() calls - msync() can
    force a write-back to disk on demand, though MAP_SHARED writes get
    flushed back eventually on their own regardless. See
    `code/59_MmapFile/01_fileBackedMmap.c`.

Copy-on-write (COW) after fork():

*   Right after fork(), the child's address space is a logical copy of the
    parent's, but the kernel doesn't actually duplicate physical memory
    immediately - both processes' page tables point at the very same
    physical pages, marked read-only. The first write to a given page by
    either process triggers a page fault (the same mechanism behind demand
    paging in `57_Paging/02_demandPaging.c`), and only then does the kernel
    copy just that one page to a new physical frame for whichever process
    wrote to it - which is exactly why the parent's value stays unchanged
    even though the child modified "the same" variable. See
    `code/60_CopyOnWrite/01_forkCow.c`.
