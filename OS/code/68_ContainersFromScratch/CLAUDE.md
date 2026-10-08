# 68_ContainersFromScratch

Containers from first principles: namespaces via clone(2) and cgroup v2 inspection, with a senior Q&A.

## Files
- `01_namespaces.c` - clone() into new user, PID, UTS and mount namespaces without root; shows the child is PID 1 with its own hostname
- `02_cgroupInspect.c` - reads /proc/self/cgroup and cgroup v2 limit/accounting files under /sys/fs/cgroup
- `NOTES.md` - namespaces, cgroups v2, filesystem and images, isolation hardening, "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_namespaces.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/68_ContainersFromScratch/` (git-ignored).

## Key concepts / interview angles
- A container is an ordinary process with namespaces (what it can see), cgroups (what it can use), a root filesystem and reduced privileges.
- Namespaces: pid, mnt, net, uts, ipc, user, cgroup; unprivileged user namespaces allow rootless containers.
- cgroups v2: unified hierarchy, controllers (cpu, memory, io, pids); memory.max triggers cgroup OOM.
- Hardening: seccomp, capabilities, LSMs (AppArmor/SELinux), read-only rootfs.
- Shared kernel means weaker isolation than VMs.

## Gotchas
- `01_namespaces.c` needs unprivileged user namespaces enabled (may be disabled by the distro/sysctl); it prints an error otherwise.
- `02_cgroupInspect.c` expects cgroup v2 mounted at `/sys/fs/cgroup`; creating a demo cgroup (see its printed instructions) needs root.

## Related
- `../47_HypervisorsAndVirtualMachines`
- `../46_KernelMemoryAllocatorsAndVirtualization`
- `../05_ResourceLimits`
- `../../../SystemDesign/topics/35_ContainersAndKubernetesBasics`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
