# 82_FileSystemExtentsAndPathLookup

Extent-based file mapping with a block bitmap, and path resolution with a trie.

## Files
- `01_extentsAndBitmap.c` - ctz first-zero allocation from a bitmap, extent list growth and merge, file offset to disk block lookup, interleaving fragmentation.
- `02_pathTrie.c` - Trie of path components: add and resolve /usr/bin/gcc style paths.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_extentsAndBitmap.c -o /tmp/x -lm && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/82_FileSystemExtentsAndPathLookup/` (git-ignored).

## Key concepts / interview angles
- **Free space:** a bitmap per block group (ext4) or B-trees of free extents (XFS). Word-at-a-time scan with count-trailing-zeros; search for a run of zeros to get contiguous space; keep a start hint, group by locality.
- **File mapping:** block-pointer files (ext2/3, FAT) need one pointer per block (a 1 GB file = 262 144 pointers). An **extent** = (first block, length); sequential files are a few extents (ext4, XFS, Btrfs, NTFS). ext4 keeps 4 extents in the inode then an extent B-tree. Offset -> disk block = binary search in the extent list; allocation continuing where the last ended **merges** into the extent. Delayed allocation + `fallocate` preserve contiguity; many extents = fragmentation.
- **Path lookup:** resolve one component at a time from the root (through the dentry cache, checking permissions). A **trie** stores shared prefixes once; compressed form = **radix tree** (IP longest-prefix match, page cache index). B+ trees keep directories/indexes shallow (3-4 disk reads for a billion keys).

## Gotchas
- The bitmap demo uses one 64-bit word; real allocators scan many words.
