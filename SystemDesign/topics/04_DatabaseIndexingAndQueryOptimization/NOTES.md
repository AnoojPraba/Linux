# Database Indexing and Query Optimization

## B-tree vs hash indexes

- **B-tree index**: keeps keys sorted, so it supports range queries
  (`WHERE age > 30`), ordering (`ORDER BY`), and prefix matches, in
  addition to equality lookups. This is the default index type in most
  relational databases because it's the more general-purpose choice.
- **Hash index**: O(1) average equality lookups (`WHERE id = 5`), but
  cannot support range queries or ordering at all since a hash destroys the
  key's natural ordering. Wins specifically for pure equality-lookup
  workloads where range queries never happen.

## Composite indexes and column order

- A composite index on `(a, b, c)` is usable for queries filtering on `a`,
  or on `a` and `b`, or on `a`, `b`, and `c` (leftmost-prefix rule) - but
  NOT for a query that filters on `b` alone, because the index is physically
  sorted by `a` first.
- Column order should generally put the most selective / most commonly
  filtered-alone column first, though equality-filtered columns before
  range-filtered columns is also a common rule of thumb.

## N+1 query problem

- Happens when code fetches a list of N parent rows, then issues one
  additional query per row to fetch related data (e.g. loop over N orders,
  querying `customer` for each one individually) - N+1 total queries where
  1 or 2 would suffice.
- Fixes: eager loading (fetch parents and their related rows together up
  front), a `JOIN` that returns everything in one query, or batching
  (`WHERE id IN (...)`) to fetch all related rows for the whole batch at once.

## EXPLAIN plans (conceptually)

- Shows how the query planner intends to execute a query: which
  index (if any) it will use, join order, and whether it expects a full
  table scan vs an index scan/seek.
- Interview-relevant takeaway: a slow query is often slow because the
  planner is doing a full table scan when an index should be used instead -
  reading a plan is about spotting that mismatch, not memorizing plan syntax.

## Costs of indexing

- Every index speeds up reads that use it but slows down writes (`INSERT`/
  `UPDATE`/`DELETE`), since each index must also be updated on every write,
  and consumes additional storage.
- Too many indexes (especially unused or overlapping ones) can make write
  throughput the bottleneck - indexing should be driven by actual query
  patterns, not "index everything just in case."
