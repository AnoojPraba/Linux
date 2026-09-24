# Combined Debugging Case Study

`01_buggyCounter.c` has two independent, deliberately seeded bugs:
- A heap buffer overflow (off-by-one in `buildRecords`).
- A data race (unprotected `sharedCounter++` across threads in
  `incrementCounter`).

This NOTES.md ties together the tools this repo covers separately -
`62_GdbWorkflow`, `63_ValgrindAndAsan`, `61_ThreadSanitizerDemo`, and
`64_PerfAndStrace` - into one walkthrough, since the
real skill is picking the *right* tool for a given symptom, not just knowing
each tool exists.

## 1. Noticing something is wrong

- Running the program repeatedly shows `sharedCounter` printing a different,
  too-low value each time (e.g. `229526` instead of the expected `400000`)
  - a classic symptom of a data race (lost updates), not a crash.
  - `perf stat`/`/usr/bin/time` (see `64_PerfAndStrace`) wouldn't diagnose
    *this* bug directly, but is the right first move when the symptom is
    "too slow" rather than "wrong answer" or "crashed" - it's a
    process-health tool, not a correctness tool.
- The heap overflow may not crash immediately (small overflows often land
  in unused heap padding), which is exactly why "it didn't crash" is not the
  same as "it's correct" - this is where the memory-safety tools below come
  in even without an obvious symptom.

## 2. GDB: first look at a crash (see `62_GdbWorkflow`)

- If the overflow happens to corrupt something that *does* crash (e.g.
  heap metadata used by a later `malloc`/`free` call), GDB is the first
  stop: run under `gdb`, reproduce the crash, then `bt` for a backtrace and
  `p` to inspect variables at the point of the crash.
- The catch: GDB shows you *where the crash happened*, which is often far
  removed from *where the corruption occurred* (heap corruption frequently
  doesn't crash until a later, unrelated allocation/free call touches the
  corrupted metadata). GDB alone can leave you staring at the wrong frame.

## 3. ASan: pinpointing the heap corruption precisely (see `63_ValgrindAndAsan`)

- Recompile with `gcc -fsanitize=address -g 01_buggyCounter.c -o buggy_asan
  -pthread` and run it.
- ASan reports a precise `heap-buffer-overflow` at the exact `records[i] =
  i * i` write that goes out of bounds, plus the exact `malloc` call site
  that allocated the too-small block - both the allocation site and the
  invalid access site in one report. This is dramatically more precise than
  GDB's "here's where it eventually crashed."

## 4. TSan: pinpointing the exact data race (see `61_ThreadSanitizerDemo`)

- Recompile with `gcc -fsanitize=thread -g 01_buggyCounter.c -o buggy_tsan
  -pthread` and run it.
- TSan reports a `data race` with two stack traces - one for each thread's
  unsynchronized read/write of `sharedCounter` in `incrementCounter` -
  naming the exact line and variable. ASan would not catch this bug (it's
  not a memory-safety violation, just a race), and GDB would only show you
  "it ran and gave a wrong number," with no indication of *why*.

## 5. Meta-lesson: match the tool to the bug class

| Symptom                          | First tool  | What it tells you                          |
|-----------------------------------|-------------|---------------------------------------------|
| Crash, need current state         | GDB         | Where it crashed and what the state is now  |
| Wrong values, possible memory bug | ASan        | Precise memory-safety violation + culprit   |
| Wrong values, multiple threads    | TSan        | Precise race + which threads/line/variable  |
| Slow, not wrong                   | perf/strace | Where time is spent (not a correctness tool)|

A senior engineer's value here isn't knowing all four tools exist - it's
recognizing from the *symptom* which one to reach for first, rather than
reaching for GDB (or worse, `printf`) for every bug regardless of class.
