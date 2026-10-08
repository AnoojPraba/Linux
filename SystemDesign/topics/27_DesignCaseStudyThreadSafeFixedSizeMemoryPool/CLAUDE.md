# 27_DesignCaseStudyThreadSafeFixedSizeMemoryPool

Case study: thread-safe fixed-size memory pool (intrusive free list, thread-local vs mutex vs lock-free CAS, the ABA problem).

## Files
- `NOTES.md` - (64 lines) sections: Requirements clarification (ask before designing); Core design; Thread-safety approaches, cheapest to most complex; What interviewers are actually listening for

## How to use this note
- Whiteboard the free-list pool, then climb the thread-safety ladder and be ready to explain ABA precisely when pushed.
- Explain why you would build it instead of using malloc (O(1), no fragmentation, locality).
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Pre-allocate a contiguous block, carve into fixed chunks, store the free-list `next` pointer inside each free chunk; allocate/free are O(1) pops/pushes.
- Thread-local pools need no synchronisation but cross-thread frees are unsupported or slow; global mutex is simple but contended; lock-free CAS avoids blocking but suffers ABA.
- ABA mitigations: tagged pointers (generation counter), hazard pointers, or reasoning that never-returned memory keeps intrusive links consistent (no double free).
- Requirements to ask: single size or size classes, allocation rate, single- or multi-thread use.

## Related
- `../25_DesignCaseStudyHFTOrderBookMatchingEngine`
- `../../../OS/code/45_CustomAllocator/02_fixedSizePoolAllocator.c`
- `../../../OS/code/66_MemoryModelLitmusTests/02_abaTaggedStack.c`
- `../../../C_Basics/code/81_MallocInternalsAndAllocators`
- `../../../Cpp/code/29_CustomAllocatorCpp`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
