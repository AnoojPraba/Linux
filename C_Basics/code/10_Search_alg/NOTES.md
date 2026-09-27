# Search Algorithms

## Ternary search

Ternary search (`04_ternarySearch.c`) splits the search range into three
parts instead of two, but it isn't a generally better sorted-array search
than binary search. Its real use case is finding the maximum or minimum of a
unimodal function (one that strictly increases then strictly decreases, or
vice versa), where comparing two interior points tells you which third of the
range can't contain the extremum.

For plain sorted-array lookup, binary search is still generally preferred:
ternary search does 2 comparisons per level (to evaluate both `mid1` and
`mid2`) versus binary search's 1 comparison per level, and splitting into 3
parts instead of 2 doesn't make up for that - the extra comparisons roughly
cancel out the smaller per-level range reduction, giving no real practical
advantage over binary search.
