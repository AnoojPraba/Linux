# 71_EbpfAndTracingBasics

Conceptual notes on Linux tracing and eBPF with bpftrace/perf/ftrace one-liners (NOTES-only).

## Files
- `NOTES.md` - tracing landscape, what eBPF is, practical one-liners (who opens files, read() latency histogram, off-CPU time), and a "Senior interviewer Q&A" section

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- eBPF programs are verified, JIT-compiled and attached to hooks (kprobes, tracepoints, uprobes, perf events, XDP) with maps for state.
- Choose the tool by question: perf (CPU sampling), ftrace (function tracing), strace (syscalls, high overhead), bpftrace/BCC (custom low-overhead aggregations).
- Always-on safety: verifier bounds loops/memory; overhead scales with event rate.
- Off-CPU analysis finds blocking, not just hot code.

## Gotchas
- One-liners need root (or CAP_BPF/CAP_PERFMON) and a kernel with BTF/bpftrace installed; nothing to compile here.

## Related
- `../72_PerformanceDebuggingMethodology`
- `../70_InterruptPathAndKernelModules`
- `../../../C_Basics/code/69_PerfAndStrace`
- `../08_SystemCalls`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
