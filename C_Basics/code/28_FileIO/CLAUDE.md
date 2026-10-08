# 28_FileIO

File I/O two ways: buffered stdio (text and binary) and raw POSIX system calls.

## Files
- `01_writeRead.c` - fopen/fprintf/fgets text round trip via /tmp/c_basics_fileio_demo.txt
- `02_binaryFreadFwrite.c` - fwrite/fread of binary records via /tmp/c_basics_binary_demo.bin
- `03_syscallIO.c` - open/write/read/close file descriptors via /tmp/c_basics_syscall_io_demo.txt

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_writeRead.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/28_FileIO/` (git-ignored).

## Key concepts / interview angles
- stdio is buffered in user space on top of the read/write syscalls; `fflush` vs `fsync` differ (user buffer vs disk).
- Check every return value (fopen, fwrite short counts, read EINTR/partial reads).
- Binary struct dumps are non-portable (padding, endianness).

## Gotchas
- Writes demo files under `/tmp/`; safe to rerun.

## Related
- `../../../OS/code/57_FileSystemStructuresAndAllocation`
- `../06_EndiannessAndByteOrder`
- `../76_ErrorHandlingAndCleanupPatterns`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
