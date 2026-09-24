# GDB Workflow

Companion demo: `01_segfaultDemo.c` - `findRecord()` returns `NULL` for an
out-of-range index, and `summarizeRecord()` dereferences it without a NULL
check. Build it with the repo's own binary (`../bin/58_GdbWorkflow/01_segfaultDemo`)
and run it with an out-of-range argument, e.g. `./01_segfaultDemo 5`, to crash it.
The transcript below is not something the agent runs interactively - it is the
walkthrough you'd type by hand to find this exact bug.

## Breakpoints and watchpoints

- `break <func>` / `b <file>:<line>` - stop execution when that function/line
  is reached. `watch <expr>` - stop when a variable's *value* changes
  (useful when you don't know which line corrupts it, only that it does).
- `break ... if <cond>` sets a conditional breakpoint (e.g. `break
  findRecord if index > 2`), avoiding a manual stop-inspect-continue loop.
- `info breakpoints` lists all breakpoints/watchpoints; `delete N` /
  `disable N` remove or pause one by number.

## Backtrace and frame navigation

- `bt` (or `where`) prints the call stack from the crash point outward -
  first thing to run after any crash or `SIGSEGV` stop.
- `frame N` / `up` / `down` move between stack frames so you can inspect
  each caller's locals with `info locals` or `p <var>`.

## Examining variables and memory

- `p <expr>` (print) evaluates and prints an expression, including struct
  members and casts, e.g. `p *record`, `p record->id`.
- `x/<n><f><u> <addr>` examines raw memory: count, format (`x`=hex,
  `d`=decimal, `s`=string, `i`=instruction), unit size (`b`/`h`/`w`/`g`).
  E.g. `x/4xw &records` dumps 4 hex words starting at `records`.
- `p/x`, `p/t` etc. reprint the last value in hex/binary.

## Stepping

- `step` (`s`) steps into function calls; `next` (`n`) steps over them
  (treats a call as one line). `finish` runs until the current function
  returns and prints its return value.
- `continue` (`c`) resumes until the next breakpoint/watchpoint/crash.

## Core dump analysis

- Enable core dumps first: `ulimit -c unlimited` (per-shell; add to
  `.bashrc` to persist), then run the crashing program normally - on
  SIGSEGV it writes a `core` (or `core.<pid>`) file in the cwd (subject to
  `/proc/sys/kernel/core_pattern`).
- Load it post-mortem: `gdb <binary> -c core` (or `gdb -c core.1234
  <binary>`) - drops you at the crash frame with `bt`/`p`/`frame` all
  available, without needing to reproduce the crash live. Essential for
  crashes that only happen in production or are hard to reproduce.

## Registers and low-level state

- `info registers` (`i r`) dumps all general-purpose registers; `p $rip` /
  `p $pc` prints just the instruction pointer, `p $rsp` the stack pointer.
  Useful when a crash lands in code with no debug symbols and `bt` is
  incomplete.

## Attaching to a running process

- `gdb -p <pid>` attaches to an already-running process (must have
  ptrace permission - same user or root, and not blocked by
  `/proc/sys/kernel/yama/ptrace_scope`). Useful for a hung or misbehaving
  long-running server without restarting it.
- `detach` releases the process to keep running; `kill` from inside gdb
  terminates it.

## Example session (finding the bug in `01_segfaultDemo.c`)

```
$ gdb ./01_segfaultDemo
(gdb) break summarizeRecord
Breakpoint 1 at 0x... : file 01_segfaultDemo.c, line 58.
(gdb) run 5
Starting program: .../01_segfaultDemo 5

Breakpoint 1, summarizeRecord (record=0x0) at 01_segfaultDemo.c:58
58          printf("record %d: %s\n", record->id, record->name);
(gdb) p record
$1 = (Record *) 0x0
```
`record` is `0x0` - already enough to know the bug is "a NULL record reached
`summarizeRecord()`", not a corrupted struct. Continuing confirms the crash:
```
(gdb) continue
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x... in summarizeRecord (record=0x0) at 01_segfaultDemo.c:58
58          printf("record %d: %s\n", record->id, record->name);
(gdb) bt
#0  summarizeRecord (record=0x0) at 01_segfaultDemo.c:58
#1  0x... in main (argc=2, argv=0x...) at 01_segfaultDemo.c:82
```
`bt` shows `main` called `summarizeRecord` directly with the bad pointer -
tracing back to `main`'s `found = findRecord(records, lookupIndex);` shows
`findRecord()`'s `NULL`-on-out-of-range return was never checked before use.
Fix: check `found != NULL` in `main` (or inside `summarizeRecord`) before
dereferencing.

## Same bug via a core dump (no live debugger session)

```
$ ulimit -c unlimited
$ ./01_segfaultDemo 5
Segmentation fault (core dumped)
$ gdb ./01_segfaultDemo -c core
(gdb) bt
#0  summarizeRecord (record=0x0) at 01_segfaultDemo.c:58
#1  0x... in main (argc=2, argv=0x...) at 01_segfaultDemo.c:82
(gdb) p record
$1 = (Record *) 0x0
```
Same conclusion, without ever attaching gdb before the crash happened.
