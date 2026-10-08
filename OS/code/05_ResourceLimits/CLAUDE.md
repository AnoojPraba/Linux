# 05_ResourceLimits

Per-process resource limits with getrlimit/setrlimit, shown by exhausting file descriptors.

## Files
- `01_rlimitFileDescriptors.c` - reads RLIMIT_NOFILE soft/hard, lowers the soft limit, opens /dev/null until EMFILE

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_rlimitFileDescriptors.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/05_ResourceLimits/` (git-ignored).

## Key concepts / interview angles
- Soft limit is enforced and can be raised up to the hard limit; only root can raise the hard limit.
- "Too many open files" (EMFILE) is a limit failure, not a bug in open().
- Other limits: RLIMIT_NPROC, RLIMIT_AS, RLIMIT_STACK, RLIMIT_CORE; `ulimit` is the shell front end; cgroups for container limits.
- Servers raise NOFILE for many connections (see the C10K discussion in `../54_IOMultiplexing`).

## Related
- `../08_SystemCalls`
- `../07_ErrnoAndErrorHandling`
- `../68_ContainersFromScratch`
- `../54_IOMultiplexing`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
