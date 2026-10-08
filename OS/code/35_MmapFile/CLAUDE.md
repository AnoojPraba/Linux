# 35_MmapFile

File-backed shared mmap: read and modify a file through memory and flush with msync.

## Files
- `01_fileBackedMmap.c` - creates /tmp/c_basics_mmap_file_demo.txt, mmap MAP_SHARED, modifies it in memory, msync(MS_SYNC), reads back

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_fileBackedMmap.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/35_MmapFile/` (git-ignored).

## Key concepts / interview angles
- File mappings page in from the page cache; MAP_SHARED writes go to the file, MAP_PRIVATE is copy-on-write.
- `msync` forces writeback; otherwise the kernel flushes later.
- Cannot extend a file by writing past its end (SIGBUS beyond EOF): `ftruncate` first.
- mmap I/O vs read/write: fewer copies, but page-fault cost and error handling by signal.
- Used by databases, loaders (`ld.so` maps libraries), shared memory.

## Gotchas
- Writes a scratch file under `/tmp/`.

## Related
- `../30_Paging`
- `../36_CopyOnWrite`
- `../55_ZeroCopyIOAndIoUring`
- `../48_IPC`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
