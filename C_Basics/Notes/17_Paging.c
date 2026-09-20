Paging is how the OS manages memory: physical RAM and each process's virtual
address space are both divided into fixed-size chunks called pages (almost
always 4 KB on x86_64 Linux - queried at runtime via sysconf(_SC_PAGESIZE),
not a compile-time constant, since it can differ across architectures). See
`code/31_Paging/01_pageSizeAndMmap.c`.

Why pages instead of just giving each process a contiguous slab of real
RAM?

*   A process's virtual addresses don't have to map to contiguous physical
    RAM - the kernel maintains a page table translating virtual page
    number -> physical frame number per process, so physical memory can be
    scattered and still look contiguous to the program using it.
*   Pages can be swapped to disk individually and brought back when
    touched again, letting the OS run more total virtual memory than
    physical RAM actually exists.
*   Per-page protection bits (readable/writable/executable) let the OS
    enforce things like "this code page is executable but not writable"
    at the hardware level, via the MMU (memory management unit) - see
    `code/30_SystemCalls/05_memoryManagementMprotect.c`, where mprotect()
    marks a page read-only and a subsequent write faults with SIGSEGV.

Demand paging: mmap()'ing a region doesn't actually assign physical memory
to it - it just reserves a range of virtual addresses. The very first
read/write to a given page triggers a page fault: the kernel notices there's
no physical frame behind that virtual page yet, assigns one on the spot,
and only then does the page actually cost real memory. This is why a large
mmap() can appear "free" until you start touching it, and why memory use
grows one page (not the whole mapping) at a time as you touch more of it.
`code/31_Paging/02_demandPaging.c` shows this directly, tracking a process's
own resident set size (RSS - the physical memory actually behind it right
now) via /proc/self/status before the mmap, right after it (unchanged),
and after touching every page (grown by roughly one page's worth of RSS
per page touched).

A page fault isn't always an error - it's also the normal mechanism behind
demand paging above. It only becomes a fatal SIGSEGV (as in the mprotect
example) when the access itself is illegal - writing to a read-only page,
or touching an address with no mapping at all.
