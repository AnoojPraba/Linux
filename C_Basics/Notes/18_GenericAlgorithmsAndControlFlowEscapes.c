Two small clusters of standard-library/language features, grouped here
because they're both about giving up strict structured control flow (or
type-specific code) for something more general.

qsort / bsearch (`<stdlib.h>`):

*   qsort and bsearch are generic - they take a `void *` array, an element
    count, an element size, and a comparator function pointer, and know
    nothing about the actual element type. All type-specific logic lives
    in the comparator (int a - int b for ints; compare whichever struct
    field matters for structs) - the same type-erasure idea as
    `03_pointers/06_voidPointer.c`, but applied to a whole algorithm instead
    of a single value.
*   bsearch requires the array already sorted by the *same* comparator -
    it has no way to verify that itself; giving it a mismatched comparator
    or an unsorted array produces silently wrong answers, not a crash.
*   See `code/19_QsortBsearch/01_qsortInts.c` and
    `code/19_QsortBsearch/02_bsearchStructs.c`.

Control-flow escape hatches:

*   goto - almost always avoidable with better-structured loops/functions,
    but has one broadly accepted use: breaking out of nested loops in one
    jump, since a plain `break` only exits the innermost loop. See
    `code/18_ControlFlowExtras/01_goto.c`.
*   assert (`<assert.h>`) - checks an invariant that should be impossible
    to violate if the rest of the program is correct, not a substitute for
    validating real (possibly-wrong) user input. Defining NDEBUG compiles
    every assert() out entirely, so an assert must never itself perform a
    side effect the program depends on. See
    `code/18_ControlFlowExtras/02_assert.c`.
*   setjmp/longjmp (`<setjmp.h>`) - C's closest thing to an exception:
    longjmp unwinds the call stack straight back to a matching setjmp
    call, skipping every intervening return, with none of C++'s automatic
    destructor cleanup along the way. See
    `code/18_ControlFlowExtras/03_setjmpLongjmp.c`.
