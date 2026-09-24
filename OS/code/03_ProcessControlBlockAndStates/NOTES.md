# Process Control Block and Process States

Notes-only: this is kernel-bookkeeping theory behind what `02_Processes` and
`04_ProcessLifecycle` already demonstrate with real `fork()`/`exec()`/`wait()`
code - it explains *what the kernel tracks* rather than adding another demo.

## Process Control Block (PCB)

- The kernel's per-process bookkeeping structure - one PCB exists for every
  process, created on `fork()`/`exec()` and freed once the parent reaps it.
  On Linux this is `struct task_struct` (also used for threads, since Linux
  threads are just task_structs sharing an address space).
- What it stores:
  - **PID** (and PPID) - the process's identity and its parent.
  - **Process state** - New/Ready/Running/Waiting/Terminated (see below).
  - **Program counter** - address of the next instruction to execute, saved
    here whenever the process isn't currently running on a CPU.
  - **CPU registers** - full register file, saved/restored on every context
    switch so the process resumes exactly where it left off.
  - **Memory management info** - page tables, segment info, base/limit
    registers - the mapping from this process's virtual address space to
    physical memory.
  - **Open file descriptor table** - which files/sockets/pipes are open and
    their current offsets.
  - **Scheduling info** - priority, nice value, scheduling class/policy,
    time slice remaining.
  - **Accounting info** - CPU time used, wall-clock start time, resource
    limits (see `05_ResourceLimits`).
- Why it matters: a context switch is, mechanically, "save the current
  process's registers/PC into its PCB, load the next process's registers/PC
  from its PCB." The PCB is what makes a process resumable.

## Process Table

- The kernel's collection of all PCBs - conceptually an array/list, one
  entry per active process, indexed (or hashed) by PID.
- `02_Processes`/`04_ProcessLifecycle`'s use of `/proc/[pid]/...` is a
  userspace window into this table - each `/proc/[pid]` directory exposes
  fields straight out of that process's PCB (state, memory maps, fd list,
  etc.).

## Process states and the state diagram

```
        admitted            dispatch (scheduler picks it)
  New ----------> Ready ------------------------> Running
                    ^                                 |  |
                    |    interrupt / time-slice        |  |
                    +------------------------ expiry --+  |
                                                           | I/O or event wait
                                                           v
                                              Waiting/Blocked
                                                           |
                                                exit        I/O or event
                                                           v completion
                                              Terminated  (back to Ready)
```

- **New -> Ready**: *admitted* - the long-term scheduler (conceptually;
  see `22_SchedulingConceptsDeepDive`) lets the process into the ready
  queue once resources are available.
- **Ready -> Running**: *dispatched* - the short-term/CPU scheduler picks
  this process and the dispatcher context-switches onto it.
- **Running -> Ready**: *interrupt or time-slice expiry* - preempted (e.g.
  Round Robin's quantum runs out, or a higher-priority process becomes
  ready) without the process's cooperation.
- **Running -> Waiting/Blocked**: the process itself requests something
  that isn't immediately available - a blocking syscall (I/O, `wait()`,
  a lock) - and voluntarily gives up the CPU.
- **Waiting/Blocked -> Ready**: the awaited I/O or event completes; the
  process becomes runnable again but does not get the CPU immediately -
  it re-enters the ready queue.
- **Running -> Terminated**: the process calls `exit()` (or is killed by a
  signal) - `04_ProcessLifecycle`'s zombie state is "Terminated but not
  yet reaped": the PCB survives until the parent calls `wait()`/`waitpid()`
  to collect the exit status, at which point the PCB is finally freed.

## Seeing real process state

`/proc/[pid]/stat`'s third field is the state character: `R` (running),
`S` (sleeping/interruptible wait), `D` (uninterruptible wait, usually I/O),
`T` (stopped), `Z` (zombie/terminated-not-reaped). This is the same
PCB `state` field rendered as text - a quick way to confirm the state
diagram against a real process:

```sh
$ awk '{print $3}' /proc/self/stat
R
```

## Cross-references

- `02_Processes` - `fork()`/`exec()` create the PCB and process-table entry
  this folder describes; this folder explains what's actually inside it.
- `04_ProcessLifecycle` - the zombie/orphan demos are concrete instances of
  the Terminated state (zombie) and of PPID reparenting to init on orphan.
- `23_ProcessScheduling`/`24_CPUScheduling` - use the scheduling-info field
  of the PCB (priority/nice) to decide Ready -> Running transitions.
- `22_SchedulingConceptsDeepDive` - the long/short/medium-term scheduler
  distinction referenced in the New -> Ready and Ready <-> "swapped out"
  transitions.
