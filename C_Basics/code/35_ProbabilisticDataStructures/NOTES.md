# Probabilistic Data Structures

Trade exactness for tiny, fixed memory (independent of the number of items
seen) and O(1)-ish per-operation cost - useful when the exact structure
(e.g. a hash set holding every item) would be too large to keep in memory.

## Bloom filter (`01_bloomFilter.c`)

- A bit array plus K independent hash functions. `add(item)` sets the K bits
  the item hashes to; `mightContain(item)` checks whether all K of those
  bits are set.
- False positives are possible (other items' bits happened to cover this
  item's bits too, by chance); false negatives are not (a bit this item
  needs is only ever cleared by clearing the whole filter) - so the answer
  is always "definitely not present" or "maybe present", never "definitely
  present" with certainty and never a false "definitely not present".
- Classic use case: a cheap "have I possibly seen this before" pre-check
  in front of an expensive lookup - e.g. checking a Bloom filter before a
  disk read on a cache miss (skip the disk entirely on "definitely not
  present"), or deduplicating a huge stream of items without storing every
  item seen so far.
- Deletion is hard in the basic Bloom filter: clearing a bit to "remove" an
  item might also clear a bit another item's presence depends on, silently
  turning a false negative loose. The standard fix is a Counting Bloom
  Filter (small counters instead of single bits, so a bit is only cleared
  when its counter hits zero) - not implemented here, just worth knowing it
  exists as the answer to "how would you support delete."

## Count-Min Sketch (`02_countMinSketch.c`)

- A 2D array of counters, D rows by W columns, each row using its own
  independent hash function. `add(item)` increments one counter per row (at
  that row's hash of the item); `estimate(item)` returns the *minimum* of
  the item's D counters.
- Collisions can only inflate a counter (another item sharing that
  row/column adds to it), never deflate one, so the minimum across
  independent rows is the best available estimate: it can overestimate the
  true count but never underestimate it.
- Classic use case: approximate frequency counting / "heavy hitters" (top-K
  most frequent items) over a high-volume stream - e.g. most-requested URLs
  in network traffic or clickstream analytics - where storing an exact
  per-item counter for everything seen would need too much memory.

## HyperLogLog (conceptual only, not implemented)

- Answers a different question than the two structures above: not "have I
  seen this item" or "how many times," but "how many *distinct* items have
  I seen" (cardinality estimation) - e.g. counting unique visitors in a
  huge log stream.
- Works by hashing each item and tracking, per bucket, the maximum number
  of leading zero bits seen in any hash so far; the intuition is that
  seeing a hash with `k` leading zero bits is a roughly `1/2^k` chance
  event, so the maximum `k` observed across many items implies roughly
  `2^k` distinct items were hashed. Averaging this estimate across many
  buckets (instead of one global counter) is what makes the estimate
  stable with small, fixed memory.
- Contrast with the two structures above: Bloom filter answers "is X in the
  set" (membership), Count-Min Sketch answers "how many times has X
  occurred" (frequency), HyperLogLog answers "how many distinct items have
  occurred at all" (cardinality) - three different questions, each solved
  by a different tiny-fixed-memory sketch.
