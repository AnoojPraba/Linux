A system call is the only way user-space code can ask the kernel to do
something on its behalf - open a file, spawn a process, allocate memory,
send a signal. Ordinary function calls stay in user-space; a system call
crosses into kernel-space, which is why it's slower than a regular call and
why the kernel gets to validate/police everything crossing that boundary.

libc functions like fopen() or malloc() usually aren't syscalls themselves -
they're layered on top of one or more syscalls (fopen -> open, malloc ->
mmap/brk as needed), adding buffering, formatting, or bookkeeping the raw
syscall doesn't provide. `code/44_SystemCalls/01_libcWrapperVsRawSyscall.c`
shows this directly: getpid() and syscall(SYS_getpid) do the same thing,
one through a portable wrapper, one by kernel syscall number.

How a failure is reported: almost every syscall signals failure through its
return value (typically -1) and leaves the specific reason in the global
errno - the return value alone can't distinguish "file missing" from
"permission denied". Save errno into a local variable immediately after the
failing call - another library call in between (even a successful one, like
printf) can silently overwrite it. See
`code/44_SystemCalls/02_errnoAndPerror.c`.

The syscalls in this repo, grouped by what they operate on:

*   File system - operating on files/directories and their metadata:
    `open`/`read`/`write`/`close`/`lseek` (file *contents*,
    `code/26_FileIO/03_syscallIO.c`) and `stat`/`mkdir`/`rmdir`/`creat`/
    `unlink` (file *metadata and directory entries*,
    `code/44_SystemCalls/03_fileSystemCalls.c`).
*   Process control - creating, replacing, and waiting on processes:
    `fork`/`waitpid` (`code/45_Processes/01_forkBasics.c`) and
    `execvp` (`code/45_Processes/02_execFamily.c`).
*   Memory management - controlling a process's virtual address space:
    `mmap`/`munmap` (`code/13_DynamicMemory/`, `code/57_Paging/`) and
    `mprotect`, which changes an already-mapped region's access
    permissions at the page level - see
    `code/44_SystemCalls/05_memoryManagementMprotect.c`, where writing to
    a page after `mprotect`'ing it read-only faults with SIGSEGV.
*   IPC (inter-process communication) - letting separate processes
    exchange data or coordinate: `pipe` (`code/66_IPC/01_pipeCommunication.c`),
    `mmap` with `MAP_SHARED` (`code/66_IPC/02_sharedMemory.c`), and
    process-shared semaphores (`code/66_IPC/03_processSemaphore.c`).
*   Device management - talking to devices through operations that don't
    fit the read/write model: `ioctl`, the catch-all syscall for
    device-specific requests (each device type defines its own request
    codes) - see `code/44_SystemCalls/04_deviceManagementIoctl.c`, which
    asks the terminal driver for its window size.
