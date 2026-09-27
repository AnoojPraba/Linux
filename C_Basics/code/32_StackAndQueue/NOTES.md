# Stack and Queue

- Stack (LIFO): last element in is the first out. Use cases: function call
  stacks, undo/redo history, expression evaluation and balanced-bracket
  checking, backtracking (DFS uses an implicit or explicit stack).
- Queue (FIFO): first element in is the first out. Use cases: task
  scheduling, BFS traversal (see `43_Graph`), producer/consumer buffering,
  print/job queues.
- Array vs linked-list backing:
  - Array-based: contiguous memory, good cache locality, no per-element
    allocation, but a fixed capacity (or requires costly resize/copy).
  - Linked-list-based: grows without a fixed limit and never needs a bulk
    resize, but each element pays for a heap allocation and pointer, and has
    worse cache locality (pointer chasing).
- Why naive circular-queue full/empty detection is tricky: with only
  `front`/`rear` indices, a full queue and an empty queue can both end up
  with `front == rear`, since the indices wrap via modulo. Fixes are either
  tracking a separate `count` (used here) or sacrificing one slot so the
  array is considered full when `(rear + 1) % capacity == front`.
- Deque (double-ended queue) generalizes both: pushing/popping only at the
  front reduces to a stack, only at the back reduces to a queue.
- Valid Parentheses (`06_validParentheses.c`): one of the single most
  commonly asked stack interview questions. Push every opening bracket;
  on a closing bracket, it must match whatever is currently on top of the
  stack (pop-and-match). The string is valid iff every closing bracket
  found a match and the stack ends empty - a non-empty stack at the end
  means some opening bracket was never closed.
- Min Stack (`07_minStack.c`): supports push/pop/top/getMin all in O(1) by
  maintaining a second auxiliary stack that tracks the running minimum at
  each level - every push also pushes the new min (the pushed value, or
  the current aux top if that's smaller) onto the aux stack, so getMin()
  is just an O(1) peek and pop/push keep both stacks in lockstep.
- UART RX/TX ring buffer (`08_uartRingBuffer.c`): the classic embedded
  producer/consumer pattern - a circular buffer that an ISR fills with
  incoming bytes (`bufferPush`) while the main loop drains it (`bufferPop`).
  This exists because the ISR must return as fast as possible: copying one
  byte into a buffer and returning immediately is cheap, whereas doing the
  actual (potentially slow) processing of that byte inside the interrupt
  handler risks missing subsequent interrupts or blocking other interrupts
  for too long. The actual processing happens later, in the main loop, at
  its own pace. Full/empty detection reuses the same `count`-field technique
  as `03_arrayQueue.c` in this folder, rather than the front==rear ambiguity
  trap described above. In real firmware the buffer's read/write index
  variables must be declared `volatile`, since they are written from
  interrupt context and read from main-loop context (or vice versa) - see
  `../../OS/code/11_VolatileVsAtomicEmbedded` for exactly this
  ISR-shared-state scenario. This file simulates the ISR as a plain function
  call rather than a real hardware interrupt, since real interrupts aren't
  available to trigger directly in this environment.
