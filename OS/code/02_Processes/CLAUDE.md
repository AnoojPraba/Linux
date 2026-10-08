# 02_Processes

Process creation basics: fork() duplication and exec* replacing the process image.

## Files
- `01_forkBasics.c` - fork returns twice; getpid/getppid; each process has its own copy of the address space
- `02_execFamily.c` - child calls execvp("echo", ...) which replaces its image; parent waits

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_forkBasics.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/02_Processes/` (git-ignored).

## Key concepts / interview angles
- fork returns 0 in the child and the child PID in the parent; copy-on-write makes it cheap (see `../36_CopyOnWrite`).
- exec never returns on success; fork+exec is the shell model, `posix_spawn` and `vfork` are alternatives.
- File descriptors are inherited across fork and exec unless close-on-exec.
- Always check fork failure (EAGAIN) and `wait` for children.

## Related
- `../04_ProcessLifecycle`
- `../03_ProcessControlBlockAndStates`
- `../36_CopyOnWrite`
- `../48_IPC`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
