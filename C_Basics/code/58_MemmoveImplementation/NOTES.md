# memmove vs memcpy on overlapping regions

Cross-references `57_StrictAliasing` and `03_pointers`
for the general theme of pointer-arithmetic edge cases the standard library
quietly relies on.

## Why memcpy is undefined behavior on overlap

- A naive `memcpy` implementation copies forward, byte by byte, from `src[0]`
  to `dest[0]`, `src[1]` to `dest[1]`, and so on.
- If the regions overlap and `dest > src` (e.g. shifting bytes right within
  the same buffer), by the time the loop reaches a given `src[i]`, an
  earlier iteration may have already overwritten it via `dest[i - k]` for
  some `k > 0` that mapped onto the same address. The copy silently reads
  *its own already-written garbage* instead of the original source bytes.
- Because this failure mode depends on the relative addresses and the
  specific implementation's copy order (some implementations copy
  backward, some use SIMD in chunks), the C standard simply declares the
  behavior undefined when regions passed to `memcpy` overlap - callers must
  use `memmove` if overlap is possible.

## How memmove avoids it: direction detection

- `memmove` first compares `dest` and `src`:
  - `dest < src`: copying forward is safe, since every byte is read before
    any write could reach that same address (the write always trails the
    read).
  - `dest > src`: copying forward would read-after-write source bytes
    whenever the regions actually overlap, so `memmove` instead copies
    **backward**, from the last byte down to the first - now every write
    trails the corresponding read in the opposite direction, which is safe.
  - `dest == src`: no-op either way.
- `01_myMemmove.c` implements exactly this direction check with two simple
  byte-by-byte loops (forward and backward).

## Real-world optimization note

- Production implementations (e.g. glibc) don't stop at byte-by-byte
  copying - when `dest` and `src` share the same alignment relative to a
  machine word, they copy `uint64_t`-sized (or wider, with SIMD) chunks at a
  time, handling any unaligned leading/trailing bytes separately. This is a
  large speed win over per-byte copying but doesn't change the core
  direction-detection logic above, which remains correctness-critical
  regardless of chunk size.
