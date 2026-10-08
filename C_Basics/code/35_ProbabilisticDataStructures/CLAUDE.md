# 35_ProbabilisticDataStructures

Space-efficient approximate structures: Bloom filter and Count-Min Sketch implemented; HyperLogLog covered conceptually.

## Files
- `01_bloomFilter.c` - bit array with K hash functions: add and possibly-contains
- `02_countMinSketch.c` - D x W counter matrix for approximate frequency counts
- `NOTES.md` - Bloom filter, Count-Min Sketch, HyperLogLog (concept only, not implemented)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_bloomFilter.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/35_ProbabilisticDataStructures/` (git-ignored).

## Key concepts / interview angles
- Bloom: no false negatives, tunable false-positive rate (bits per element, number of hashes), no deletion in the basic form (counting Bloom filters add it).
- Count-Min: overestimates only; take the minimum across rows; useful for heavy hitters / top-K.
- HyperLogLog estimates distinct count (cardinality) from the longest run of leading zeros per bucket.
- Typical uses: pre-check before an expensive disk/DB lookup, network telemetry, LSM-tree read filters.

## Related
- `../34_HashTable`
- `../../../SystemDesign/topics/03_CachingStrategies`
- `../../../SystemDesign/topics/30_LSMTreesAndStorageEngines`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
