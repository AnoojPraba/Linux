# 04_ProcessLifecycle

Zombie, orphan and daemon processes: what happens after fork when parent and child exit in different orders.

## Files
- `01_zombieProcess.c` - child exits while parent sleeps 2 s without wait(); observe state Z with ps
- `02_orphanProcess.c` - parent exits first; child is re-parented to init/subreaper
- `03_daemonProcess.c` - classic double-fork daemonisation; appends one line to /tmp/c_basics_daemon_demo.log then exits

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_zombieProcess.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/04_ProcessLifecycle/` (git-ignored).

## Key concepts / interview angles
- Zombie: exited child whose status was not collected with `wait()`; costs a PID/table slot, cannot be killed (reap it or kill the parent).
- Orphan: re-parented to PID 1 or the nearest subreaper (`PR_SET_CHILD_SUBREAPER`).
- Daemonisation: fork, `setsid`, fork again, `chdir("/")`, reset umask, close/redirect fds; modern services use systemd instead.
- Handle `SIGCHLD` or use `waitpid(WNOHANG)` loops to avoid zombies.

## Gotchas
- Programs sleep for 1-2 seconds; run `ps -o pid,ppid,state,cmd -p <pid>` in another terminal during the zombie demo.
- `03_daemonProcess.c` forks twice, appends one line to `/tmp/c_basics_daemon_demo.log` and the daemon child exits immediately, so nothing lingers; the shell prompt returns at once.

## Related
- `../02_Processes`
- `../06_SignalHandling`
- `../03_ProcessControlBlockAndStates`
- `../../../SystemDesign/topics/23_DeploymentStrategies`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
