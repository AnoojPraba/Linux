# Embedded Reliability and Power Management

Watchdog timers and chip sleep states are hardware/peripheral features - a
meaningful demo needs real microcontroller hardware (or at least a specific
chip's datasheet/register set) to program against, not something a plain
userspace C program on a dev box can exercise, similar to `42_HardwareBuses`.
Switch debouncing, however, is demonstrable in plain C by simulating a noisy
input signal - see `01_switchDebounce.c` and the section below.

## Watchdog timers

- A hardware timer that software must periodically "kick" (a.k.a. "pet" -
  reset the countdown) within a timeout window. If the software hangs,
  deadlocks, or otherwise stops kicking it, the watchdog's countdown expires
  and it fires a hardware reset of the system.
- This is a last-resort recovery mechanism specifically for embedded systems
  that can't rely on a human to notice a hang and power-cycle the device -
  e.g. a remote sensor node, an automotive ECU, or any unattended deployed
  device. The watchdog gives it a way to recover from firmware bugs (an
  infinite loop, a deadlock, a stuck peripheral wait) on its own.
- **Simple watchdog**: must be kicked at least once before the timeout
  expires - kicking early is always fine, so it only catches "software
  stopped running entirely" style hangs.
- **Windowed watchdog**: must be kicked within a specific window - not
  before the window opens, and not after it closes. Kicking too early is
  itself treated as a fault. This catches a wider class of bugs than a
  simple watchdog: a task that is still alive but running far too fast (e.g.
  stuck in a tight loop skipping real work) can still kick the watchdog in
  time with a simple watchdog, but a windowed watchdog will trip if kicks
  arrive with the wrong timing, not just if they stop entirely.

## Low-power modes / sleep states

- Microcontrollers typically offer several depths of sleep, trading wake-up
  latency against power savings:
  - **Sleep**: the CPU core is halted but peripherals and RAM stay powered
    and retain their state - wakes quickly (often just one interrupt away
    from resuming exactly where it left off).
  - **Stop / deep sleep**: most clocks are stopped (including higher-speed
    oscillators), RAM is retained, but more of the chip is powered down -
    wake-up is slower since clocks/PLLs need to restart before normal
    execution resumes.
  - **Standby / shutdown**: nearly the entire chip is powered off; only a
    tiny always-on domain (e.g. a real-time clock and a wake pin/timer)
    survives. RAM is often lost entirely, so firmware must re-initialize
    from scratch on wake (sometimes re-reading saved state from
    non-volatile storage). This gives the lowest power draw but the slowest,
    most expensive wake-up.
- The fundamental tradeoff is always wake-up latency vs power savings; there
  is no mode that is simply "better" - choosing one means picking a point on
  that curve based on the application's duty cycle (how often it needs to
  wake up) and latency requirements (how quickly it must respond once
  woken).

## Stack overflow and limited RAM constraints

- Embedded RAM is commonly measured in KB, not GB, which makes stack
  overflow a much more realistic day-to-day concern than on a desktop
  system: a deeply recursive function, or one with large local
  variables/arrays, can overrun its stack and corrupt adjacent memory - the
  heap, or (in an RTOS with no MMU protection between tasks) another task's
  stack entirely. See `../../C_Basics/code/14_Recursion` for recursion depth/
  stack-growth basics and `../../C_Basics/code/32_StackAndQueue` for the
  stack data structure itself; on a memory-constrained embedded target,
  those same growth patterns can be the difference between working firmware
  and silent memory corruption.
- Also see this folder's task-stack-sizing discussion in
  `23_RTOSConceptsAndTaskScheduling` (if running under an RTOS): each task's
  own small fixed stack makes this constraint even tighter than a single
  bare-metal main-loop program with one shared stack.
- **Static stack usage analysis**: rather than only relying on runtime
  high-water-mark checks, GCC's `-fstack-usage` flag emits a `.su` file per
  compiled function listing its worst-case stack frame size, letting you sum
  up the worst-case stack depth of a call chain at build time and verify it
  stays within budget on a memory-constrained target - useful as a build-time
  gate before code ever runs on hardware.

## Switch/button debouncing

- Mechanical switch contacts don't make or break cleanly: when a button is
  pressed or released, the physical contacts vibrate/bounce for a few
  milliseconds before settling, producing several rapid true/false electrical
  transitions instead of one clean edge. Naively treating every raw
  transition as a real state change causes a single physical press to look
  like multiple presses to software.
- **Polling-based debounce**: sample the pin periodically (e.g. every 1-5 ms)
  and require several consecutive identical readings before accepting the new
  state as real - any bounce back to the old value resets the run of
  consecutive readings. `01_switchDebounce.c` demonstrates this counter-based
  approach against a simulated bouncy signal (an array of readings standing
  in for successive polled samples), requiring `STABLE_READS_REQUIRED`
  consecutive matching samples before accepting a transition, and correctly
  filters out the simulated bounce to report only the genuine press and
  release.
- **Interrupt-based debounce with a timer**: instead of polling, an edge
  interrupt fires on the very first transition; the handler starts (or
  restarts) a short one-shot timer instead of immediately accepting the new
  state. If another edge interrupt arrives before the timer expires, the
  timer is restarted again. Only when the timer finally expires without a
  further edge is the new state accepted as real. This avoids continuous
  polling but still needs a timer resource per debounced input.
- **Hardware debouncing**: an RC low-pass filter (a resistor and capacitor
  smoothing out the fast bounce transitions before they reach the input pin)
  or a dedicated debounce IC can solve the same problem in hardware, trading
  extra board components for zero software complexity - a common choice when
  spare timer/CPU cycles are scarcer than board space or bill-of-materials
  cost.
