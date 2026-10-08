# 58_FilesystemInternals

Inodes, hard links and symbolic links observed with stat()/lstat().

## Files
- `01_inodesAndLinks.c` - creates /tmp/73_target.txt plus a hard link and a symlink, prints inode numbers and link counts
- `NOTES.md` - filesystem internals notes

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_inodesAndLinks.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/58_FilesystemInternals/` (git-ignored).

## Key concepts / interview angles
- An inode holds metadata and block pointers, not the name; directory entries map names to inode numbers.
- Hard links share an inode (link count increments), cannot cross filesystems or link directories; symlinks are separate files holding a path (can dangle).
- `stat` follows symlinks, `lstat` does not; deleting = unlink, data freed when the count reaches zero and no fd is open.
- Journaling and metadata consistency (fsck) are common follow-ups.

## Gotchas
- Creates and removes `/tmp/73_target.txt`, `/tmp/73_hardlink.txt`, `/tmp/73_symlink.txt`; names are fixed, so concurrent runs can collide.

## Related
- `../57_FileSystemStructuresAndAllocation`
- `../08_SystemCalls/03_fileSystemCalls.c`
- `../35_MmapFile`
- `../44_FirmwareUpdateAndFlashStorage`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
