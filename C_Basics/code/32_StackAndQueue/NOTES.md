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
