# array notes

## True 2D array vs array of pointers (`05_2dArrayVsArrayOfPointers.c`)

- `int matrix[ROWS][COLS]` is a single contiguous block of
  `ROWS * COLS * sizeof(int)` bytes, laid out row-major. `matrix[i][j]` is
  computed by the compiler as `matrix + i*COLS + j` - pure pointer
  arithmetic, no extra dereference beyond the final element access.
  Consecutive row addresses differ by exactly `COLS * sizeof(int)`.
- `int *matrix[ROWS]`, with each row separately `malloc`'d, is NOT
  contiguous - each row can land anywhere on the heap. `matrix[i][j]` means
  `*(matrix[i] + j)`: first load the row pointer from `matrix[i]`, then
  offset into that block - an actual pointer dereference, not just
  arithmetic.

Practical implications:
- True 2D arrays are more cache-friendly (one contiguous block, good
  spatial locality) and need only one allocation/deallocation, but their
  dimensions must be known at compile time (or via a C99+ VLA, which has
  its own stack-size/lifetime tradeoffs - see
  `../22_AdvancedArrays/01_variableLengthArray.c`).
- Arrays of pointers allow genuinely ragged/jagged arrays (different row
  lengths) and runtime-determined dimensions via simple per-row
  `malloc`/`free`, at the cost of scattered memory and an extra
  dereference per access.
