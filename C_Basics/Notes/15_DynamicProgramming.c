Dynamic programming (DP) isn't a data structure - it's a technique for
solving a problem by breaking it into overlapping sub-problems, solving each
sub-problem exactly once, and reusing that answer instead of recomputing it.

Two things a problem needs before DP applies at all:

*   Optimal substructure - the best answer to the whole problem can be
    built from the best answers to its sub-problems (e.g. the best value a
    knapsack can hold is built from the best value smaller knapsacks/item
    sets could hold).
*   Overlapping sub-problems - naive recursion would solve the *same*
    sub-problem many times. This is the part that's actually expensive to
    fix - see `code/09_Recursion/02_fibonacci.c`'s fibNaive, which is
    O(2^n) purely because fib(n-2) gets recomputed from scratch inside both
    the fib(n-1) and fib(n-2) branches, over and over.

If a problem has overlapping sub-problems but recomputing them is cheap
anyway (or there's no actual overlap), DP doesn't help - it's specifically
the "same work done repeatedly" case that DP is fixing, by caching answers
instead of recomputing them.

Two ways to apply it, both computing the same answers:

*   Top-down / memoization - still write the plain recursive solution, but
    check a cache before recursing, and fill the cache after. Easy to
    derive directly from the naive recursive version; only computes the
    sub-problems actually needed for this input.
*   Bottom-up / tabulation - no recursion: fill a table iteratively from
    the base cases upward, in an order that guarantees each cell's
    dependencies are already filled in. Same time complexity as
    memoization, but avoids recursion's call-stack depth entirely, and
    often has less per-call overhead.

The table's shape follows directly from what a sub-problem needs to be
described:

*   1D table - a sub-problem described by a single number (e.g.
    "fibonacci of n" only needs n to identify it).
*   2D table - a sub-problem described by two numbers (e.g. knapsack needs
    both "how many items considered" and "remaining capacity"; longest
    common subsequence needs a position in *each* of the two strings).

See the runnable examples:
*   `code/29_DynamicProgramming/01_fibMemoVsTabulation.c` - the same
    problem solved both ways, side by side, to see the direct correspondence
    between them.
*   `code/29_DynamicProgramming/02_knapsack.c` - a 2D table, and the
    "skip this item vs. take it" choice that fills each cell.
*   `code/29_DynamicProgramming/03_longestCommonSubsequence.c` - a 2D table
    over two strings' positions instead of over items/capacity.
