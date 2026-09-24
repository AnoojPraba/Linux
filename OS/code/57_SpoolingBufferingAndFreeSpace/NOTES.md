# Spooling, Buffering, and Free Space Management

## Buffering
- Temporarily holds data in memory while it's transferred between two
  devices/processes that operate at different speeds or on different
  timing (e.g. read-ahead buffering for disk I/O, a socket's kernel
  send/receive buffer smoothing out network jitter).
- Smooths out a speed mismatch for a SINGLE ongoing data stream/job - it's
  about pacing one flow of data, not queuing independent units of work.

## Spooling (Simultaneous Peripheral Operations On-Line)
- Queues entire jobs' output to disk (classically: print jobs) so a slow
  device (a printer) can be fed at its own pace while the producing
  process continues immediately instead of blocking on the slow device.
- Key difference from buffering: spooling holds COMPLETE, INDEPENDENT jobs
  - potentially from multiple different sources/processes - in a queue to
  be consumed one at a time; buffering smooths a single ongoing stream for
  one job. A spooler is job-queue-shaped; a buffer is a pipe-shaped
  smoothing mechanism.
- Modern example: a print spooler (CUPS on Linux) lets several applications
  each queue a print job independently; the spooler serializes them to the
  one physical printer, letting each app's `print()` call return instantly
  instead of blocking until the printer is done.

## Free space management
Tracking which disk blocks are currently unused so new allocations know
where they can go.

- **Bitmap / bit vector**: one bit per block (1 = allocated, 0 = free, or
  vice versa). Simple and compact; finding a free block (or a run of N free
  blocks) needs scanning the bitmap, though word-at-a-time bit tricks (e.g.
  finding the first nonzero word) make this fast in practice. Easy to find
  contiguous runs by scanning for runs of zero bits.
- **Linked list of free blocks**: each free block itself stores a pointer to
  the next free block; the filesystem just keeps a pointer to the head of
  the list. Needs no extra dedicated space beyond the free blocks
  themselves, but finding a large contiguous run is slow - you must
  traverse the list checking for numerically-adjacent blocks, and
  allocation/free is inherently one-block-at-a-time.
- **Grouping**: a variation on the linked list - the first free block in
  the list stores the addresses of several (e.g. n-1) other free blocks
  instead of just one "next" pointer, and the last of those addresses
  points to another block that continues the pattern. Amortizes the cost of
  traversing the free list since one lookup yields many free block
  addresses at once.
- **Counting**: exploits the fact that free space tends to occur in runs
  (several free blocks are contiguous on disk when a file is deleted) - so
  instead of one entry per free block, each entry stores the address of the
  first free block in a run plus a count of how many free blocks follow it
  contiguously. Much more compact than a per-block list/bitmap when
  freedom is clustered.

## Related
- `55_FilesystemInternals` covers inodes and hard/symbolic links (the
  metadata/naming side of a filesystem).
- `54_FileSystemStructuresAndAllocation` covers directory structures and
  how a file's OWN data blocks are tracked (contiguous/linked/indexed
  allocation) - free space management here is the complementary problem of
  tracking which blocks are NOT currently used by any file.
