# 77_FairSchedulerAndHeapScheduler

CFS-style run queue ordered by virtual runtime (AVL tree with remove-leftmost) and a min-heap priority scheduler.

## Files
- `01_cfsStyleRunqueue.c` - Tasks with weights 1024/1024/512 picked by smallest vruntime; the lighter task gets half the CPU.
- `02_heapPriorityScheduler.c` - Binary min-heap picking the shortest burst first.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_cfsStyleRunqueue.c -o /tmp/x -lm && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/77_FairSchedulerAndHeapScheduler/` (git-ignored).

## Key concepts / interview angles
- **CFS** gives each task a **vruntime** = CPU used scaled by weight (`delta * 1024 / weight`; nice 0 = weight 1024, ~25% per nice step). Rule: run the task with the **smallest vruntime**. Runnable tasks live in a **red-black tree** by vruntime; the leftmost node is cached so pick is O(1); enqueue/dequeue O(log n). A woken sleeper is placed near `min_vruntime`. Per-CPU trees + load balancing. RT classes (`SCHED_FIFO/RR/DEADLINE`) always run first. Newer kernels: EEVDF (still a tree, picks by eligibility + virtual deadline).
- **Why a tree and not a heap:** a heap finds the minimum in O(1) but cannot remove an arbitrary task (blocking, priority change) cheaply. **Why balanced:** vruntime keys arrive increasing, an unbalanced BST degenerates to O(n). Red-black rotates less than AVL on updates.
- **Heap scheduler:** array heap, children `2i+1`/`2i+2`, push sift-up, pop sift-down; used for SJF/priority/deadline selection and for timer queues. A few fixed priority levels are better served by an array of FIFO queues + a bitmap of non-empty queues (Linux O(1) scheduler, `find_first_bit`).

## Gotchas
- The AVL stand-in demonstrates the mechanism; the kernel uses rb-trees (`rbtree.h`).
