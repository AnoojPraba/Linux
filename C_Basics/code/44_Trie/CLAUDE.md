# 44_Trie

Prefix tree: insert/search, prefix queries (autocomplete) and recursive deletion with node pruning.

## Files
- `01_insertSearch.c` - one child slot per character; insert creates nodes lazily; exact-word search
- `02_prefixSearch.c` - startsWith / prefix walk, the autocomplete and spell-check use case
- `03_deletion.c` - recursive delete that frees nodes only when they have no children and are not another word's end

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_insertSearch.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/44_Trie/` (git-ignored).

## Key concepts / interview angles
- Lookup/insert cost is O(L) in key length, independent of the number of keys.
- Memory is the trade-off: 26 (or 256) pointers per node; compressed tries/radix trees, ternary search trees reduce it.
- Deletion must not remove nodes still used by other words (check end-of-word flag and children).
- Uses: autocomplete, dictionaries, IP routing (longest-prefix match).

## Related
- `../34_HashTable`
- `../11_StringPatternMatching`
- `../../../SystemDesign/topics/34_DesignCaseStudySearchAndInvertedIndex`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
