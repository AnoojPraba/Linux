# 08_SystemCalls

The syscall boundary: libc wrapper vs raw syscall, errno/perror, file-system metadata calls, ioctl and mprotect.

## Files
- `01_libcWrapperVsRawSyscall.c` - getpid() vs syscall(SYS_getpid)
- `02_errnoAndPerror.c` - failure convention: -1 return plus errno detail
- `03_fileSystemCalls.c` - mkdir/creat/rename/unlink under /tmp/c_basics_syscall_dir_demo
- `04_deviceManagementIoctl.c` - ioctl(TIOCGWINSZ) asks the terminal driver for the window size
- `05_memoryManagementMprotect.c` - mmap a page, mprotect it read-only, catch the SIGSEGV on write with a safe handler

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_libcWrapperVsRawSyscall.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/08_SystemCalls/` (git-ignored).

## Key concepts / interview angles
- A syscall switches user to kernel mode (trap instruction); cost is mode switch plus cache/TLB effects; vDSO avoids it for `gettimeofday`/`clock_gettime`.
- libc wrappers set errno from the negative kernel return.
- `ioctl` is the catch-all for device-specific control; `mprotect` changes page permissions (basis of guard pages and JITs).
- Handler for SIGSEGV may only use async-signal-safe calls (`write`).
- Trace with `strace` (see C_Basics 69_PerfAndStrace).

## Gotchas
- `04_deviceManagementIoctl.c` falls back to a message when stdout is not a terminal (e.g. when piped).
- `05_memoryManagementMprotect.c` intentionally triggers a SIGSEGV that its own handler catches.
- `03_fileSystemCalls.c` creates and removes files under `/tmp/`.

## Related
- `../06_SignalHandling`
- `../35_MmapFile`
- `../55_ZeroCopyIOAndIoUring`
- `../../../C_Basics/code/28_FileIO/03_syscallIO.c`
- `../../../C_Basics/code/69_PerfAndStrace`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
