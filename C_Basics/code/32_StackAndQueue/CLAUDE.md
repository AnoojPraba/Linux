# 32_StackAndQueue

Stacks, queues and deques (array and linked-list backed) plus the classic interview problems valid parentheses, min-stack and an embedded UART ring buffer.

## Files
- `01_arrayStack.c` - fixed-capacity array stack (init/push/pop/peek)
- `02_linkedListStack.c` - linked-list stack, no fixed capacity, node layout as in 33_LinkedList
- `03_arrayQueue.c` - circular array queue; a count field disambiguates full vs empty
- `04_linkedListQueue.c` - head/tail pointers give O(1) enqueue and dequeue
- `05_deque.c` - circular array deque with modulo wraparound at both ends
- `06_validParentheses.c` - bracket matching with a stack
- `07_minStack.c` - O(1) push/pop/top/getMin via an auxiliary min stack
- `08_uartRingBuffer.c` - ISR-producer / main-consumer ring buffer; notes that indices would be volatile in real firmware
- `NOTES.md` - LIFO vs FIFO use cases, array vs list backing, circular-queue full/empty trap, deque, parentheses, min stack, UART buffer

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_arrayStack.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/32_StackAndQueue/` (git-ignored).

## Key concepts / interview angles
- Circular queue: `front == rear` is ambiguous (full or empty); use a count field or leave one slot unused.
- Array backing: cache friendly but bounded; list backing: unbounded, per-node malloc cost.
- Min stack stores the running minimum alongside each pushed value for O(1) getMin.
- Single-producer/single-consumer ring buffers in ISR code need volatile/atomics and memory ordering, not just volatile (see OS lock-free ring buffer).
- Stack use cases: call stack emulation, DFS, expression evaluation, undo.

## Related
- `../33_LinkedList`
- `../../../OS/code/37_LockFreeRingBuffer` - lock-free SPSC version
- `../16_ConstVolatile`
- `../43_Graph` - BFS uses a queue, DFS a stack

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
