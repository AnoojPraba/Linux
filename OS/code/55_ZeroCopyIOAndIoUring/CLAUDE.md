# 55_ZeroCopyIOAndIoUring

Conceptual notes on zero-copy I/O (sendfile, splice), hugepages and io_uring (NOTES-only).

## Files
- `NOTES.md` - sendfile/splice zero-copy, hugepages, io_uring submission/completion rings

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Classic read+write copies data kernel to user and back; `sendfile`/`splice` keep it in the kernel, `MSG_ZEROCOPY` and DMA remove more copies.
- Hugepages cut TLB misses for big buffers (and are needed by some DMA paths).
- io_uring: shared submission and completion rings, batched syscalls, optional kernel polling (SQPOLL); true async for files, not just sockets.
- Zero-copy is not free: pinning pages, completion notification, small-message overhead.

## Related
- `../54_IOMultiplexing`
- `../67_EpollInDepth`
- `../35_MmapFile`
- `../61_IOManagementPollingInterruptsDMA`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
