# String Pattern Matching

- Naive search is O(n*m) worst case: for each of the ~n starting positions it
  can compare up to m characters before failing. A repetitive pattern like
  "aaab" against a text of "a"s (e.g. "aaaa...aab") triggers this - almost
  every position matches m-1 characters before the final mismatch.
- KMP avoids redundant comparisons using the failure function (LPS array):
  `lps[i]` is the length of the longest proper prefix of `pattern[0..i]`
  that is also a suffix of it. On a mismatch, instead of restarting the
  pattern pointer from 0, KMP jumps it to `lps[j - 1]` - reusing the fact
  that the already-matched characters tell us exactly how much of the
  pattern could still line up. The text pointer never moves backward, so
  total time is O(n + m).
- Rabin-Karp uses a rolling hash: the hash of the next text window is
  computed from the current window's hash in O(1) (subtract the leading
  character's weighted contribution, shift, add the trailing character),
  rather than rehashing the whole window each time. Hashes are compared
  first; only on a hash match does it fall back to a full character
  comparison (handles hash collisions, keeping the algorithm correct).
  Average case O(n + m), worst case O(n*m) if collisions are frequent.
- When each is preferred:
  - Naive: fine for short/one-off searches where simplicity beats
    performance.
  - KMP: guaranteed linear time regardless of input, general-purpose choice
    when worst-case matters.
  - Rabin-Karp: shines when searching for multiple patterns at once (hash
    each pattern once, then scan the text once comparing against all of
    them) - e.g. plagiarism detection / duplicate-substring style matching.
