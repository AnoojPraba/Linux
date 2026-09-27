# RTOS Concepts and Task Scheduling

Notes-only: real RTOS primitives (task notifications, priority-inheritance
mutexes) are hard to demonstrate meaningfully without an actual RTOS kernel
(FreeRTOS/Zephyr/ThreadX) running on real or emulated hardware - a plain
userspace pthread program on Linux can't show bounded-latency preemption or
per-task fixed stacks in a way that's genuinely different from `09_Threads`.

## RTOS vs general-purpose OS

- An RTOS (Real-Time Operating System) optimizes for DETERMINISTIC, bounded
  response time, not throughput or fairness. A general-purpose OS scheduler
  (e.g. Linux CFS) tries to share the CPU fairly across many processes and
  maximize overall throughput; an RTOS scheduler (e.g. FreeRTOS) instead
  guarantees that a high-priority task preempts any lower-priority task
  within a bounded, predictable amount of time - correctness of the whole
  system depends on that bound holding, not on average-case throughput.
- "Real-time" means predictable/bounded latency, not necessarily "fast" -
  a hard-real-time system that misses a deadline has failed, even if it's
  otherwise fast on average.

## Task model

- RTOS "tasks" (the RTOS term, distinct from an OS "process") typically
  share a single flat address space - many microcontrollers have no MMU, so
  there is no hardware-enforced isolation between tasks the way there is
  between OS processes.
- Each task has its own stack, but all tasks execute in the same address
  space and can (bugs aside) read/write each other's memory directly.
- Scheduling is strict fixed-priority preemptive: the highest-priority ready
  task always runs; a lower-priority task never runs while a higher-priority
  one is ready. This is the practical implementation of the theoretical
  real-time scheduling models in `22_SchedulingConceptsDeepDive` and
  `26_AdvancedSchedulingAlgorithms` (Rate Monotonic Scheduling assigns fixed
  priorities by period; Earliest Deadline First is dynamic-priority) - an
  RTOS like FreeRTOS is where RMS/EDF theory actually gets implemented as a
  scheduler, rather than staying an analysis technique.

## RTOS-specific primitives

- **Task notifications**: a lightweight, fast alternative to a full
  semaphore or queue for the common case of "wake up this one specific
  task". Each task has a single built-in notification value/flag; sending a
  notification is cheaper than allocating and managing a queue/semaphore
  object, which matters on memory- and cycle-constrained microcontrollers.
  Used heavily in FreeRTOS for simple task-to-task signaling.
- **Priority inheritance mutexes**: the RTOS-level fix for priority
  inversion (see `16_PriorityInversion` for the general problem: a
  high-priority task blocked on a lock held by a low-priority task gets
  stuck behind an unrelated medium-priority task). A priority-inheritance
  mutex temporarily boosts the lock-holding low-priority task to the
  priority of the highest-priority task waiting on it, so a medium-priority
  task can no longer preempt the holder and indirectly starve the
  high-priority waiter. Most RTOSes offer this as the default or an opt-in
  mutex type.
- **Software timers**: callback-based, one-shot or periodic timers that run
  their callback in a dedicated timer task (a "daemon" task), not in
  interrupt context - this keeps the callback able to do normal task-level
  things (call blocking APIs, use normal task priorities) that an interrupt
  handler couldn't safely do.

## Stack sizing per task

- Each task's stack is small and fixed-size, often only a few hundred bytes
  to a few KB - much smaller than a desktop process's default stack (often
  MB-sized). This makes stack overflow a real, common bug: a task with a
  deep call chain or large local arrays can silently overrun into adjacent
  memory (another task's stack, or the heap), corrupting unrelated state
  with no MMU to catch it.
- Because of this, most RTOSes provide stack high-water-mark checking (an
  API to query how close a task has ever come to overflowing its stack) and
  overflow-detection hooks (e.g. FreeRTOS's `configCHECK_FOR_STACK_OVERFLOW`,
  which can pattern-fill each stack at creation and later check the pattern
  is intact, or check the stack pointer against the stack's known bounds on
  every context switch).

## Real-world examples

- **FreeRTOS** - small, widely used, the canonical example of task
  notifications and configurable priority-inheritance mutexes above.
- **Zephyr** - a more full-featured RTOS (device driver model, networking
  stack, POSIX-compatibility layer) from the Linux Foundation, common in
  newer IoT/embedded designs.
- **ThreadX (Azure RTOS)** - widely deployed in industry (billions of
  devices), now maintained by Microsoft/Eclipse Foundation.
