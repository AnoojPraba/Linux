# Linked Lists

- Why deletion is trickier than insertion: insertion only has to point a new
  node at existing neighbors, but deletion must repoint an *existing*
  neighbor around the node being removed, or the list breaks. For a singly
  linked list, that means finding the previous node first (there's no way
  back from the node itself) and setting `prev->next = current->next` before
  freeing `current`. For a doubly linked list, both directions must be fixed
  up - `current->prev->next` and `current->next->prev` - or forward and
  backward traversal disagree with each other.
- Edge cases that are easy to get wrong: deleting the head (the caller's
  head pointer itself must change, not just some node's `next`), deleting
  the tail (the new tail's `next` must become NULL, not point at freed
  memory), and deleting the only remaining node (must leave the list empty
  rather than a dangling self-reference or freed pointer still in use). Get
  any of these wrong and the result is either a corrupted list, a memory
  leak (unlinked but never freed), or a dangling pointer (freed but still
  reachable).
- Circular lists use cases:
  - Round-robin CPU scheduling: the ready queue is naturally circular - after
    the last process gets its time slice, the scheduler wraps back to the
    first. See `../../OS/code/25_CPUScheduling/03_roundRobin.c`, which
    implements this cycling-through-processes behavior (there using an array
    index that wraps with modulo rather than a circular list, but the
    semantics - "after the last one, go back to the first" - are identical
    to what `07_circularSinglyLinkedList.c` and `08_circularDoublyLinkedList.c`
    demonstrate with actual next/prev pointers).
  - Buffer/playlist "next wraps to first" semantics: a circular buffer or a
    playlist on repeat needs "next after the last item is the first item"
    without any special-case code at the boundary - the circular list's
    structure makes wraparound the default behavior instead of something the
    caller has to check for.
- Merging two sorted lists (`09_mergeSortedLists.c`): use a dummy head node
  to avoid special-casing which list contributes the new head, then walk
  both lists comparing their current nodes, always attaching the smaller
  one to the merged tail. Once one list is exhausted, attach whatever
  remains of the other wholesale - it's already sorted, so no further
  comparisons are needed.
- Finding the middle of a list (`10_middleOfList.c`): slow/fast pointers
  again, but a different technique from cycle detection (`04_loopFinder.c`)
  even though both use the same two pointers. Cycle detection watches for
  `slow == fast` to happen at all; finding the middle just exploits the
  relative speed difference (fast covers ground twice as fast as slow) so
  that when fast runs out of list, slow is sitting at the midpoint. For an
  even-length list this implementation returns the second of the two
  middle nodes, the common convention.
- General pattern: slow/fast pointers show up repeatedly in linked-list
  problems beyond just these two - cycle detection, finding the middle, and
  finding the nth-from-end node (advance a lead pointer n steps first, then
  move both together until the lead hits the end) are all variations on
  keeping two pointers moving through the list at different rates or
  offsets to extract positional information in a single pass.
