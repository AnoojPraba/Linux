# 12_Sorting

Sorting algorithms from O(n^2) basics to quick/merge/heap, counting and radix sort, with a NOTES.md on complexity, stability and in-place trade-offs (frequent interview ground).

## Files
- `01_quickSort.c` - quicksort with partition and swap helper
- `02_mergeSort.c` - top-down merge sort with a temporary merge buffer
- `03_heapSort.c` - sift-down max-heap sort, in place
- `04_bubbleSort.c` - adjacent swaps, shrinking inner bound
- `05_selectionSort.c` - one swap per pass
- `06_insertionSort.c` - grows a sorted prefix by shifting
- `07_countingSort.c` - prefix sums over a known key range, stable back-to-front placement
- `08_radixSort.c` - LSD radix using counting sort per digit
- `Simplesort.cpp` - C++ (cin/cout) program: reads N from stdin (max 100), fills with rand(), sorts
- `NOTES.md` - average/worst complexity, stability, in-place vs extra memory, when each is preferred, plus O(n^2) sorts and counting/radix

## Build and run
- C files: see single-file command below. C++ file: `g++ -std=c++17 -Wall -Wextra Simplesort.cpp -o /tmp/x && echo 5 | /tmp/x`.
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_quickSort.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/12_Sorting/` (git-ignored).

## Key concepts / interview angles
- Quick: avg O(n log n), worst O(n^2) (bad pivot), in place, not stable; stdlib uses introsort-style fallback.
- Merge: always O(n log n), stable, O(n) extra buffer; right for linked lists and external sort.
- Heap: O(n log n) worst, O(1) extra, not stable, poor cache locality.
- Bubble and insertion are stable; selection is not; insertion is best for small/nearly sorted data.
- Counting/radix beat comparison sorts (O(n+k)) when keys are bounded integers; radix needs a stable inner sort.

## Gotchas
- `Simplesort.cpp` is C++ and reads stdin; the Makefile compiles it with g++, the others with gcc.

## Related
- `../21_QsortBsearch` - stdlib qsort
- `../41_Heap` - heap structure used by heap sort
- `../45_SegmentTreeAndFenwickTree`
- `../../../Cpp/code/19_STLAlgorithms`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
