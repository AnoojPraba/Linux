# 33_LinkedList

Linked list family: singly, doubly and circular lists with insertion/deletion, plus classic problems (reverse, cycle detection, merge, middle).

## Files
- `01_singlyLinkedList.c` - basic singly linked list append/print
- `02_reverseLinkedList.c` - in-place reversal; returns/updates the new head
- `03_doublyLinkedList.c` - prev pointer enables backward traversal
- `04_loopFinder.c` - Floyd tortoise-and-hare cycle detection
- `05_singlyLinkedListDeletion.c` - delete by key including the head case (double pointer)
- `06_doublyLinkedListDeletion.c` - doubly linked deletion with prev/next relinking
- `07_circularSinglyLinkedList.c` - tail points back to head
- `08_circularDoublyLinkedList.c` - circular list with both links
- `09_mergeSortedLists.c` - merge two sorted lists using a dummy head node
- `10_middleOfList.c` - slow/fast pointer to find the middle
- `NOTES.md` - why deletion is trickier than insertion, edge cases, circular-list uses (round-robin), merge and slow/fast patterns

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_singlyLinkedList.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/33_LinkedList/` (git-ignored).

## Key concepts / interview angles
- Deleting the head changes the caller's pointer: return the new head or pass `Node **`.
- Floyd: slow moves 1, fast moves 2; they meet inside a cycle; reset one pointer to the head to find the cycle start.
- Dummy head node removes special cases when merging or deleting.
- Slow/fast pointers also give the middle node and the k-th from end.
- Circular lists model round-robin scheduling and ring buffers.
- Always null-check and free removed nodes (valgrind).

## Gotchas
- The code follows a project rule of avoiding double pointers except where the head must be reassigned (see comments in 02 and 05).

## Related
- `../32_StackAndQueue`
- `../34_HashTable` - chaining reuses the node pattern
- `../74_ContainerOfAndIntrusiveLists` - kernel-style intrusive lists
- `../37_LRUCache`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
