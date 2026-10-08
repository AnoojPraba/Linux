# Containers From First Principles

**A container is not a kernel object.** It is an ordinary process (tree) with:
1. **namespaces** - restrict what it can SEE,
2. **cgroups** - restrict what it can USE,
3. a **root filesystem** (image layers via overlayfs, `pivot_root`),
4. **security filters** - capabilities, seccomp-bpf, LSMs (AppArmor/SELinux).
Same kernel as the host (unlike a VM, `../47_HypervisorsAndVirtualMachines`) -
so fast start, low overhead, weaker isolation.

## Namespaces (`01_namespaces.c`)
| Namespace | Flag | Isolates |
|---|---|---|
| Mount | `CLONE_NEWNS` | mount table / filesystem view |
| PID | `CLONE_NEWPID` | PID numbering - child is PID 1, can't see/signal host PIDs |
| Network | `CLONE_NEWNET` | interfaces, routing, iptables, ports (`veth` pairs + bridge connect it) |
| UTS | `CLONE_NEWUTS` | hostname/domain |
| IPC | `CLONE_NEWIPC` | SysV IPC, POSIX queues |
| User | `CLONE_NEWUSER` | uid/gid mapping: root inside = unprivileged outside (rootless containers) |
| Cgroup | `CLONE_NEWCGROUP` | view of the cgroup tree |
| Time | `CLONE_NEWTIME` | boot/monotonic clock offsets |
- APIs: `clone(2)` with flags (create), `unshare(2)` (move the CALLING process
  into new ones), `setns(2)` (join an existing one - `docker exec`, `nsenter`).
  Inspect: `ls -l /proc/PID/ns`, `lsns`, `unshare --pid --fork --mount-proc`.
- **PID 1 duty:** the namespace's init must reap zombies and forward signals;
  otherwise orphaned children pile up and `docker stop` hangs (use `tini`,
  `--init`, or `dumb-init`). Signals to PID 1 get default-action protection:
  no handler = signal ignored!
- **Why the user namespace matters for security:** unprivileged users can
  create namespaces and get "root" with capabilities only over resources the
  namespace owns. It also enlarges kernel attack surface (many distros restrict it).

## cgroups v2 (`02_cgroupInspect.c`)
- A single hierarchy under `/sys/fs/cgroup`; controllers: `cpu` (weights,
  `cpu.max` quota/period), `memory` (`memory.max` hard limit -> cgroup OOM
  kill, `memory.high` throttle, `memory.swap.max`), `io` (`io.max`), `pids`
  (`pids.max` stops fork bombs), `cpuset` (CPU/NUMA pinning).
- CPU quota vs shares: `cpu.max "50000 100000"` is a HARD cap (50% of one CPU
  per 100 ms period - can cause latency spikes via throttling); `cpu.weight`
  is proportional only under contention.
- Memory: counts page cache too; hitting `memory.max` triggers reclaim, then
  OOM kill inside the cgroup (not the whole host). JVM/Go must be container-aware
  (`-XX:+UseContainerSupport`, `GOMEMLIMIT`, `GOMAXPROCS`) or they size heaps/thread
  pools from the HOST's resources.
- v1 had one hierarchy per controller (messy); v2 unified.
- On this Pi only `cpu` and `pids` are delegated to the user cgroup and
  `memory` is not enabled (the demo prints `(unreadable)`) - a reminder that
  controller availability depends on the kernel cmdline/systemd delegation.

## Filesystem and images
- Image = stack of read-only layers (tarballs); the runtime mounts them with
  **overlayfs** (lowerdir layers + upperdir writable + merged view) - copy-up on
  first write. `chroot` is escapable; runtimes use `pivot_root` into a new mount
  namespace and unmount the old root.
- Volumes/bind mounts bypass the overlay for persistent or shared data.

## Isolation hardening
- Drop capabilities (`CAP_SYS_ADMIN` is the "new root"), `no_new_privs`, seccomp
  syscall allow-list (Docker's default blocks ~44 syscalls), read-only rootfs,
  user namespaces / rootless mode, AppArmor/SELinux profiles, no `--privileged`,
  don't mount the Docker socket.
- Containers share the kernel: a kernel exploit escapes; use gVisor/Kata/
  Firecracker (microVMs) for hostile multi-tenant workloads.

## Senior interviewer Q&A
**Q: What is a container, technically? How does it differ from a VM?**
A: A process (group) isolated by namespaces and limited by cgroups, with its
own root filesystem, running on the host kernel. A VM runs its own kernel on
virtualized hardware via a hypervisor: stronger isolation, heavier. Containers
start in milliseconds and have near-native performance; they share kernel
attack surface.

**Q: What happens when you run `docker run`?**
A: Pull/unpack image layers, set up overlayfs, create namespaces (+ veth for
net), create a cgroup and apply limits, set capabilities and seccomp, `pivot_root`
into the rootfs, then `exec` the entrypoint as PID 1 of the new PID namespace
(runc does exactly this).

**Q: Why does my containerized Java service get OOM-killed even though heap is
below the limit?**
A: Container memory counts everything: heap, metaspace, thread stacks, direct
buffers, JIT, mmap'd files and page cache. `-Xmx` equal to the limit leaves no
headroom. Size heap to ~60-75% of the limit and watch RSS vs `memory.current`.

**Q: A container shows 100% CPU throttling but low CPU usage - why?**
A: CFS quota enforcement: the cgroup used its quota early in each 100 ms period
and was paused for the remainder. Multi-threaded apps burn quota in parallel.
Raise the limit, remove it in favor of requests/weights, or tune thread counts.

**Q: How do two containers talk on one host?**
A: Each has a network namespace with a `veth` pair; the host end attaches to a
bridge (`docker0`) with NAT/iptables or nftables rules; or a shared netns (pods
in Kubernetes share one netns for localhost).

**Q: How do you debug a container that has no tools in its image?**
A: `nsenter -t PID -n -m` from the host, `kubectl debug` with an ephemeral
container sharing the target's namespaces, or inspect `/proc/PID/root` and
`/proc/PID/ns` from outside.

**Q: What are the security limits of containers?**
A: Shared kernel; privilege escalation through kernel bugs, `--privileged`,
mounted host paths/socket, excessive capabilities; mitigated by seccomp, user
namespaces, read-only rootfs, and sandboxed runtimes.
