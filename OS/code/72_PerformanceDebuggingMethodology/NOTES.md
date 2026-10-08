# Performance Debugging Methodology

## Workflow ("the service is slow")
1. **Define the problem precisely:** what is slow (latency/throughput/tail),
   since when, for whom, what changed (deploy, traffic, data size, dependency)?
   Percentiles (p50/p99/p999), not averages. Reproduce or capture a baseline.
2. **USE method per resource (Gregg):** Utilization, Saturation, Errors for CPU,
   memory, disk, network, and software resources (locks, thread pools, queues,
   connection pools, file descriptors). Saturation (queueing) causes latency.
3. **Locate the bottleneck, top-down:** app metrics -> process (`top -H`,
   `pidstat`) -> system (`vmstat`, `iostat -x`, `sar`, `ss -s`) -> trace/profile.
4. **Hypothesize, measure, change ONE thing, re-measure.** Prove the win
   with the same benchmark; keep before/after numbers.
5. **Fix the biggest thing first (Amdahl's law):** a 50% function improved 2x is
   25% faster overall; a 2% function improved 100x is 2%.

## Triage by symptom
| Symptom | Likely cause | Tools |
|---|---|---|
| High user CPU | hot loop, bad algorithm, serialization/JSON, regex, spin | `perf top/record`, flame graph, `pidstat -u` |
| High system CPU | syscalls, context switches, page faults, network stack | `strace -c`, `perf trace`, `vmstat` (cs, sy), `pidstat -w` |
| High iowait / low CPU | disk saturation, sync writes, fsync, swap | `iostat -x`, `iotop`, `biolatency`, `vmstat si/so` |
| Low CPU but slow | blocked on locks, I/O, network, downstream, GC pause | off-CPU analysis, `strace -f -e futex`, thread dumps, `ss -ti` |
| Latency spikes (periodic) | GC, cron, compaction, THP/khugepaged, CPU throttling (cgroup), swap, noisy neighbor | `perf sched`, `runqlat`, cgroup `cpu.stat` throttled, `dmesg` |
| Memory growth | leak vs fragmentation vs cache growth | `/proc/PID/smaps_rollup`, `valgrind massif`, heaptrack, `malloc_stats` (`../../../C_Basics/code/81_MallocInternalsAndAllocators`) |
| Scales poorly with cores | lock contention, false sharing, shared counters, NUMA | `perf c2c`, `perf lock`, `mpstat -P ALL`, `numastat` (`../39_FalseSharing`, `../41_NUMABasics`) |
| Many CLOSE_WAIT / TIME_WAIT / retransmits | app not closing, short connections, network loss | `ss -tan`, `netstat -s` (`../69_TcpDeepDive`) |

## Profiling tools and what they tell you
- **Sampling profilers** (`perf record -F 99 -g`): where CPU time goes; low
  overhead; need symbols and frame pointers or DWARF (`-fno-omit-frame-pointer`,
  `-g`). View as a **flame graph**: width = time, stack = call chain; look for
  wide plateaus.
- **PMU counters** (`perf stat`): IPC (instructions per cycle: < 1 suggests
  stalls on memory), cache-misses, branch-misses, context switches, page faults.
- **Tracing** (`strace`, `perf trace`, eBPF - `../71_EbpfAndTracingBasics`):
  specific events/latencies; `strace` slows the target 10-100x - not for prod.
- **Off-CPU profiling:** where threads WAIT - essential when CPU is idle but
  requests are slow.
- **Microbenchmarks:** warm-up, repeat, pin CPU, defeat dead-code elimination,
  report distributions. See `../../../C_Basics/code/78_BranchHintsPrefetchAndCacheLayout`.

## Typical C-level culprits (items 1-2 are the demo, `01_slowService.c`)
1. **Algorithmic:** `strlen` in a loop condition -> O(n^2). Fix: hoist or scan to NUL.
2. **Syscall overhead:** one `write` per small record. Fix: buffer/batch
   (`writev`, stdio buffering, `io_uring`). Signature: `strace -c` shows a huge
   call count; sys time >> user time.
3. **Allocation churn (not in the demo):** per-request malloc/free of big
   buffers costs page faults + zeroing, and below glibc's dynamic mmap
   threshold it is cheap (I tried: glibc raised the threshold after the first
   free, so 3000 x 1 MB allocs took ~2 ms and showed nothing). Real symptoms
   appear with first-touch page faults, cross-thread frees, or fragmentation -
   confirm with `perf stat -e page-faults` / `strace -c` before "fixing" it.
   Fix: reuse buffers/pools/arenas.
4. Others: lock contention (shorten critical sections, shard, per-thread data),
   cache-unfriendly layout (AoS vs SoA, pointer chasing), false sharing, branch
   mispredicts, unnecessary copies, N+1 downstream calls, synchronous logging,
   default thread-pool sizes, DNS lookups on the hot path, TCP Nagle/delayed ACK.

## Senior interviewer Q&A
**Q: Walk me through debugging a latency regression after a deploy.**
A: Compare before/after: diff the change, check dashboards (RED/USE) for the
affected percentile, bisect by feature flag or canary, reproduce with load
test, profile both versions (CPU flame graphs + off-CPU), look for new locks,
allocations, downstream calls, or config changes. Roll back first if impact is
high, investigate afterwards.

**Q: CPU is at 100%. Is that bad?**
A: Not by itself. Check run-queue length (saturation) and whether the work is
useful: spinning, GC, interrupts, or kernel time? Latency under 100% CPU
depends on queueing; capacity planning targets headroom (e.g. 60-70%).

**Q: How do you profile in production safely?**
A: Low-frequency sampling (`perf -F 49`), eBPF aggregation in kernel, continuous
profilers (pprof/Parca/Pyroscope), short windows, avoid `strace`/`valgrind`;
always have frame pointers/symbols available and a rollback path.

**Q: How do you tell lock contention from CPU-bound work?**
A: Contention shows low CPU despite many runnable threads, `futex` syscall
storms, threads blocked in `pthread_mutex_lock` in thread dumps/gdb `thread
apply all bt`, and off-CPU flame graphs dominated by lock waits (`perf lock`,
`offcputime`).

**Q: Tail latency is bad but average is fine - why?**
A: Queueing, GC/compaction pauses, retries, head-of-line blocking, noisy
neighbors, fan-out amplification (p99 of the slowest of N calls), CPU
throttling, cold caches. Mitigate with hedged requests, timeouts, load
shedding, isolation, and by measuring percentiles.

**Q: What does IPC (instructions per cycle) tell you?**
A: Roughly how well the core is fed. IPC well below 1 on a modern CPU means
stalls (cache misses, branch mispredicts, dependency chains); high IPC means
you're compute-bound and need fewer instructions (algorithm, vectorization).
