# 78_TimerWheel

A hashed timing wheel: O(1) insert, cancel and per-tick expiry for many timeouts.

## Files
- `01_hashedTimerWheel.c` - Buckets per future tick plus a rounds counter for timers beyond one revolution.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_hashedTimerWheel.c -o /tmp/x -lm && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/78_TimerWheel/` (git-ignored).

## Key concepts / interview angles
- Kernels hold huge numbers of timers (TCP retransmit, I/O timeouts, watchdogs), mostly cancelled before they fire. Need cheap insert, cancel and "what expires now".
- **Min-heap:** O(1) peek, O(log n) insert/delete, awkward arbitrary cancel. **Wheel** (Varghese & Lauck): array of buckets, one per future tick; timer due in `d` ticks goes to `(now + d) % SLOTS`; each tick advances one slot and fires its list; list per bucket makes insert and cancel O(1).
- **Hashed wheel:** each timer carries `rounds = (delay - 1) / SLOTS`, decremented on each visit; fires when 0. **Hierarchical wheel** (Linux): several levels of 64 slots at coarser resolution, far timers **cascade** down as time approaches. Cost: coarse levels fire slightly late - fine for timeouts.
- Precise events use `hrtimers` (red-black tree by expiry). Rule: wheel for many coarse, often-cancelled timeouts; tree/heap for fewer exact timers.

## Gotchas
- Off-by-one: the rounds count must be (delay - 1) / SLOTS or a timer a full revolution away fires a revolution late.
