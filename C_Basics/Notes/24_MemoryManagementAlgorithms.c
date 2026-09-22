Two more algorithm families, extending the paging concepts in
`Notes/17_Paging.c` and `code/57_Paging/` from "what a page is" to "how the
OS decides what to evict or allocate".

Page replacement (which page to evict when physical memory is full and a
new page needs a frame):

*   FIFO - evict whichever page has been in a frame the longest, regardless
    of how recently or often it was actually used. Simple (a single
    rotating index), but can evict a page that's about to be needed again
    right after being replaced.
*   LRU (Least Recently Used) - evict whichever page was used furthest in
    the past. Needs to track recency per page, unlike FIFO's simple
    rotation, and usually approximates "keep what's probably needed again
    soon" well - but it is not guaranteed to beat FIFO on every reference
    string; `code/56_MemoryManagement/01_pageReplacement.c` runs both
    algorithms on the same reference string side by side, and that
    particular string happens to give FIFO one fewer fault than LRU, a
    real (if slightly counterintuitive) possibility, not a bug.

Contiguous memory allocation (which free block to give a new allocation
request, when memory is divided into blocks of varying free sizes):

*   First Fit - allocate the first free block large enough to hold the
    request. Fast (stops scanning at the first match), but can leave many
    small, scattered, unusable gaps (external fragmentation).
*   Best Fit - allocate the smallest free block that's still large enough,
    minimizing the leftover gap for *this* allocation. Costs a full scan
    of every block instead of stopping early, and still accumulates small
    leftover fragments over time.
*   See `code/56_MemoryManagement/02_contiguousAllocation.c`, which runs
    the same process/block sizes through both strategies so the resulting
    allocations (and which requests fail to fit anywhere) can be compared
    directly.
