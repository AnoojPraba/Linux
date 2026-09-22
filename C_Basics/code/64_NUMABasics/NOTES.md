# NUMA (Non-Uniform Memory Access) Basics

Notes-only: a proper NUMA demo needs a real multi-node NUMA machine plus
`libnuma`/`numactl` to be meaningful (measuring local vs remote latency on a
single-node dev box would just show noise) - not forced into a contrived
runnable example here.

- **UMA vs NUMA**: on a Uniform Memory Access system, every CPU has the same
  access latency to all of RAM (one shared memory controller/bus). On NUMA
  systems (common on multi-socket servers), each CPU (or CPU group, a "NUMA
  node") has its own local memory bank; accessing another node's memory
  goes over an interconnect (e.g. Intel QPI/UPI, AMD Infinity Fabric) and is
  measurably slower - "non-uniform".
- **Local vs remote access cost**: a remote access might cost roughly 1.5-2x
  (or more) the latency of a local access, depending on the interconnect and
  topology - significant for latency-sensitive or memory-bandwidth-bound
  workloads.
- **Node affinity**: pinning a thread/process to a specific NUMA node (CPU
  affinity via `sched_setaffinity()`) and allocating its memory from that
  same node keeps accesses local. `numactl --cpunodebind=N --membind=N ./prog`
  does this from the command line without code changes.
- **First-touch policy**: on Linux, a freshly `mmap`'d/`malloc`'d page is
  typically not bound to a node until it is first written - the kernel
  allocates the physical page from whichever node the touching thread is
  running on at that moment. This means the *thread that initializes* a
  buffer, not the one that allocated it, often determines which node's
  memory backs it - a common gotcha in parallel init code (e.g. one thread
  allocates and zeroes a big array before spawning workers - the whole array
  ends up local to that one thread's node, not spread out).
- **libnuma** (`numa.h`) provides programmatic control: `numa_alloc_onnode()`,
  `numa_run_on_node()`, `numa_available()` to check if the system is NUMA at
  all before using any of it.
- Interview framing: "why would you pin threads to cores on a NUMA system?"
  -> to keep each thread's memory accesses local to its own node, avoiding
  the remote-access latency penalty and interconnect contention.
- Relates to `62_FalseSharing`/`63_CacheCoherenceMESI` (cache-line
  contention) but is a different, coarser-grained effect - about which
  memory *bank* a page lives in, not which cache line.
