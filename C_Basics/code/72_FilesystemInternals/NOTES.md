# Filesystem Internals

- **Inode**: a per-file metadata structure (owner, permissions, size,
  timestamps, pointers to data blocks) identified by an inode number, unique
  per filesystem. The filename itself is NOT part of the inode - it lives in
  a directory entry mapping a name to an inode number.
- **Hard link**: a second directory entry pointing at the SAME inode number.
  Increments the inode's link count (`st_nlink`). All hard links are
  equally "real" - there's no concept of an "original" - the data is only
  actually freed when the link count reaches zero AND no process still has
  it open. Cannot span filesystems/partitions (inode numbers are only unique
  within one filesystem) and (traditionally) cannot target directories.
- **Symbolic link (symlink)**: a distinct file with its own inode, whose data
  is just the path string of a target. Can cross filesystems and point to
  directories or nonexistent targets ("dangling" symlink). `stat()` follows
  symlinks to the target; `lstat()` reports on the symlink itself.
- **`ls -l` link count column**: for a regular file, the count of hard links;
  for a directory, historically `2 + number of subdirectories` (`.` plus each
  subdirectory's `..` entry pointing back).
- **Journaling** (ext3/ext4, etc.): filesystem metadata (and optionally data)
  changes are first written to a journal log before being applied to the main
  structures. After a crash, the journal is replayed to bring the filesystem
  back to a consistent state quickly, instead of a full `fsck` scan.
  Trade-off: journaling data (not just metadata) is safer but slower.
- Demo (`01_inodesAndLinks.c`) creates a file, a hard link, and a symlink,
  then uses `stat()`/`lstat()` to show the hard link sharing an inode
  (link count 2) versus the symlink having its own distinct inode.
