# eBPF and Linux Tracing Basics

NOTES-only: `bpftrace`/`bcc`/`perf` and kernel privileges are not assumed in
this repo's setup (not installed on this Pi). Use this as the interview map and
command cheat sheet.

## The tracing landscape
| Tool | Purpose |
|---|---|
| `strace` / `ltrace` | syscalls / library calls of one process (ptrace: slow, stops the process) |
| `perf` | sampling profiler + PMU counters (cycles, cache-misses, branch-misses), `perf trace`, tracepoints |
| `ftrace` / `trace-cmd` | in-kernel function/event tracer (`/sys/kernel/tracing`) |
| `eBPF` (bpftrace, bcc, libbpf) | programmable, safe, low-overhead in-kernel tracing and networking |
| `gdb`/`rr` | interactive / record-replay debugging |
| `valgrind`/sanitizers | correctness (see `../../../C_Basics/code/68_ValgrindAndAsan`) |

## What eBPF is
- A small in-kernel virtual machine: you load a verified program into the
  running kernel and attach it to a **hook**; no kernel module, no reboot.
- **Verifier:** statically proves the program terminates (bounded loops), only
  accesses valid memory, uses only allowed helpers -> safe by construction;
  then a JIT compiles it to native code (near-native speed).
- **Hooks:** kprobes/kretprobes (kernel function entry/return), uprobes (user
  functions, e.g. `malloc` in libc), tracepoints (stable kernel events),
  `perf` events/timers (sampling), **XDP** (earliest NIC receive path - DDoS
  filtering, load balancing), `tc` (traffic control), cgroup hooks, LSM hooks,
  socket filters, syscall seccomp.
- **Maps:** kernel-resident key/value stores (hash, array, per-CPU, ring buffer,
  LRU) shared between BPF programs and user space - how counts/histograms are
  aggregated in the kernel and read cheaply.
- **CO-RE / BTF:** "compile once, run everywhere" - type info (BTF) lets one
  binary adapt struct layouts across kernel versions (libbpf).
- Cost model: in-kernel aggregation means you ship summaries, not every event;
  overhead typically a few percent or less vs ptrace-based tools at 10-100x slowdown.

## Practical one-liners (bpftrace / perf / ftrace)
```bash
# who is opening files? (syscall tracepoint)
bpftrace -e 'tracepoint:syscalls:sys_enter_openat { printf("%s %s\n", comm, str(args->filename)); }'
# latency histogram of read() syscalls
bpftrace -e 'tracepoint:syscalls:sys_enter_read { @s[tid] = nsecs; }
             tracepoint:syscalls:sys_exit_read /@s[tid]/ { @us = hist((nsecs - @s[tid]) / 1000); delete(@s[tid]); }'
# off-CPU time (why is it blocked?) - bcc: offcputime-bpfcc -p PID 10
# who calls malloc, by size (uprobe)
bpftrace -e 'uprobe:/lib/aarch64-linux-gnu/libc.so.6:malloc { @bytes = hist(arg0); }'
# CPU flame graph input
perf record -F 99 -g -p PID -- sleep 30 && perf script | stackcollapse-perf.pl | flamegraph.pl > out.svg
# cache/branch behaviour of a command
perf stat -e cycles,instructions,cache-misses,branch-misses ./prog
# TCP retransmits / connections: tcpretrans-bpfcc, tcpconnect-bpfcc, tcplife-bpfcc
# disk latency histogram: biolatency-bpfcc
```
Brendan Gregg's **BCC/bpftrace tool collection** (`execsnoop`, `opensnoop`,
`biolatency`, `runqlat`, `tcplife`, `profile`, `funccount`) covers most
first-pass questions.

## Methodology (ties to `../72_PerformanceDebuggingMethodology`)
Start with symptoms and resource metrics (USE: utilization, saturation,
errors), narrow with counters, then trace specific paths; do not start by
tracing everything.

## Security/ops notes
- Loading BPF needs `CAP_BPF`/`CAP_PERFMON` (or root); unprivileged BPF is
  usually disabled (`kernel.unprivileged_bpf_disabled`).
- Verifier rejections ("invalid mem access", "back-edge") are the common
  development friction; keep programs small, use helpers and maps.

## Senior interviewer Q&A
**Q: What is eBPF and why is it safer than a kernel module?**
A: Sandboxed bytecode verified before load (bounded, memory-safe, limited
helpers) and JIT-compiled; a bad program is rejected rather than panicking the
kernel. A module runs with full kernel privilege and any bug can crash it.

**Q: A service has random 500 ms stalls. How do you investigate?**
A: Check basics (CPU, run queue, GC logs, IO wait), then `runqlat` (scheduling
delay), `offcputime` (what it was blocked on), `biolatency`, `tcpretrans`,
lock contention (`futex` via strace/bpftrace), correlate with timestamps, and
look at tail latencies by percentile rather than averages.

**Q: strace vs perf vs eBPF?**
A: strace: precise per-process syscalls but heavy overhead (ptrace stops).
perf: sampling and hardware counters - low overhead profiling. eBPF:
programmable, can filter/aggregate in-kernel for production use.

**Q: What does XDP give you?**
A: Programmable packet processing at the driver level before the kernel
allocates an skb: drop/redirect/forward at line rate (DDoS mitigation,
L4 load balancers such as Katran/Cilium).

**Q: How would you find who is leaking file descriptors?**
A: `ls /proc/PID/fd | wc -l` over time, `lsof -p`, `strace -e openat,close`,
or a bpftrace uprobe/tracepoint on `openat`/`close` aggregating by stack
(`ustack`) to find the call site that opens without closing.
