# Zero-Copy I/O and io_uring

- NOTES.md-only: these are kernel/interface-level mechanisms whose value is
  in avoiding syscall/copy overhead at scale - a toy single-request demo
  wouldn't show that benefit, and `io_uring` needs a fairly recent kernel/
  liburing to build reliably in this repo's plain-gcc setup.

## Zero-copy I/O: `sendfile()` / `splice()`

- The traditional path for "read a file and write it to a socket" copies
  data: disk -> page cache -> user-space buffer (`read()`) -> kernel socket
  buffer (`write()`) - two copies plus two user/kernel context switches.
- `sendfile(out_fd, in_fd, offset, count)` moves data directly from one file
  descriptor's page cache to another (typically a file to a socket) inside
  the kernel, with no trip through a user-space buffer - one syscall, no
  user-space copy.
- `splice()` generalizes this via a kernel pipe buffer: it can move data
  between two arbitrary file descriptors (not just file-to-socket) as long
  as one end is a pipe, again without copying through user space.
- Both are why high-throughput file servers (e.g. serving static files)
  favor them over a `read()`/`write()` loop - less CPU spent copying, fewer
  syscalls, and no extra user-space buffer competing for cache space.

## Hugepages

- Normal pages are 4 KB on x86-64; a large process touching gigabytes of
  memory needs millions of page-table entries and puts heavy pressure on
  the TLB (see `30_PageTableEntriesAndTLB`) since each 4 KB page needs its
  own TLB entry.
- Hugepages (commonly 2 MB or 1 GB on x86-64) map a much larger region with
  a single PTE and a single TLB entry - far fewer TLB misses for
  memory-heavy workloads (databases, JVMs, HPC).
- **Transparent Huge Pages (THP)**: the kernel opportunistically backs
  anonymous memory with hugepages automatically, no application changes
  needed - but allocation/compaction can introduce latency spikes, which is
  why some latency-sensitive systems disable THP.
- **Explicit hugepages (`MAP_HUGETLB`)**: the application requests hugepages
  directly via `mmap(..., MAP_HUGETLB, ...)`, backed by a pre-reserved pool
  (`/proc/sys/vm/nr_hugepages`) - predictable, but requires that pool to be
  sized and reserved ahead of time, and pages can't be swapped.

## `io_uring`

- Traditional async I/O interfaces (`epoll` in `51_IOMultiplexing`, POSIX
  AIO) still require a syscall per operation submitted (or per readiness
  check) - at very high I/O rates, syscall overhead itself becomes a
  bottleneck.
- `io_uring` uses two ring buffers shared between user space and the kernel:
  a **submission queue (SQ)** the application writes I/O requests into, and
  a **completion queue (CQ)** the kernel writes results into - both just
  memory, no syscall needed to submit or reap in the common case (the
  kernel can poll the SQ in `SQPOLL` mode, or a single `io_uring_enter()`
  call can submit/reap a whole batch at once).
- Compare with `epoll` (`51_IOMultiplexing`): `epoll_wait()` tells you which
  file descriptors are *ready*, and the actual `read()`/`write()` is still a
  separate syscall per fd, per event. `io_uring` instead submits the actual
  I/O operation itself into the ring and gets its result back via the CQ -
  fewer syscalls overall, and it supports true async disk I/O (which
  `epoll`, oriented around sockets/pipes, does not).
- This is why `io_uring` has increasingly replaced `epoll`+thread-pool and
  POSIX AIO in high-performance servers and storage engines - lower per-op
  overhead and a unified model for both socket and file I/O.
