# Filesystem Structures and Allocation

## Directory structures
- **Single-level**: one directory for all files, all names in one flat
  namespace. Simple but names must be globally unique and there's no
  organization (fine for very small/embedded systems, impractical otherwise).
- **Two-level**: one directory per user (or namespace), each with its own
  flat file list. Solves the naming-collision problem across users but users
  still can't organize their own files into sub-groups, and sharing across
  users is awkward (paths must reference another user's directory).
- **Tree-structured**: directories can contain subdirectories, forming a
  hierarchy (what virtually every modern OS uses). Natural organization and
  navigation via absolute/relative paths, but strictly a tree - a file can
  only have one path to it, so sharing a file between two directories
  requires a full copy (no shared reference), which risks the copies
  diverging.
- **Acyclic-graph**: relaxes the tree to allow shared links (hard links /
  symlinks) so one file (or subtree) can be reachable from multiple
  directories without duplicating data. Must explicitly prevent cycles
  (e.g. a directory linking back to an ancestor) since a cycle would break
  traversal (infinite loops) and confuse reference counting / `rm -rf`-style
  recursive deletion. See `55_FilesystemInternals` for hard vs symbolic
  links, which are exactly the mechanism that turns a tree into an
  acyclic graph.

## Path names
- **Absolute path**: fully specifies location from the root (`/a/b/c`) -
  unambiguous regardless of current working directory.
- **Relative path**: specified relative to the current working directory
  (`b/c`, `../d`) - shorter but its meaning depends on context (cwd).
- **Resolution**: the kernel walks the path component-by-component, at each
  step looking up the next name as a directory entry (name -> inode number)
  in the current directory's inode, then loading that inode, until the final
  component is reached. `.` and `..` are just directory entries pointing at
  the current and parent directory's inode respectively. A symlink
  encountered mid-path is transparently substituted with its target text and
  resolution continues (bounded to avoid infinite symlink loops).

## File allocation methods
How the blocks that make up a file's data are tracked/linked on disk.

- **Contiguous allocation**: file occupies one contiguous run of blocks,
  recorded as (start block, length). Fast for both sequential AND random
  access (one seek gets you anywhere in the file via simple arithmetic).
  Downsides: external fragmentation as files are created/deleted/resized,
  and you generally must know the final file size upfront to reserve the run
  (or pay for costly compaction/relocation later).
- **Linked allocation**: each block stores a pointer to the next block in
  the file; the directory entry just holds the first block. No external
  fragmentation (any free block can be used) and files grow trivially.
  Downsides: random access is slow (must walk the chain from the start to
  reach block N), pointers waste some space per block, and reliability is
  fragile - a single corrupted pointer breaks the rest of the chain.
  (FAT filesystems use a variant: the pointers live in a separate File
  Allocation Table rather than inside each data block, which avoids losing
  data blocks to corruption but the table itself becomes a single point of
  failure.)
- **Indexed allocation**: a dedicated index block holds an array of pointers
  to all of the file's data blocks. Supports fast random access (look up
  entry N in the index) without requiring contiguous layout, at the cost of
  the extra index block(s). This is the model Unix inodes use: a fixed
  number of direct pointers for small files, plus single/double/triple
  indirect pointers (an index block whose entries point to more index
  blocks) so one inode can address arbitrarily large files while staying a
  fixed small size for small files. See `55_FilesystemInternals` for the
  inode structure itself (metadata + these block pointers).

## File access methods
- **Sequential access**: read/write proceeds in order from the start,
  advancing an implicit position each time (like a tape drive). Simple and
  matches linked allocation well (you're already walking the chain in
  order).
- **Direct / random access**: jump straight to an arbitrary block number
  (like a disk - `lseek()` then read/write). Requires an allocation method
  that supports O(1)-ish lookup of an arbitrary block, i.e. contiguous or
  indexed allocation; linked allocation makes this slow since reaching
  block N still means walking N pointers from the head.
