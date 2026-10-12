# 44_GreedyAlgorithms

Greedy algorithms in modern C++: interval scheduling and where greedy fails.

## Files
- `01_greedyActivitySelection.cpp` - Interval scheduling: sort by finish time, take what fits.
- `02_coinChangeGreedyVsDp.cpp` - Coins 4/3/1 for amount 6: greedy gives 3 coins, DP gives 2.
- `03_fractionalKnapsackAndCoins.cpp` - Greedy by ratio and by denomination: fractional knapsack (greedy is optimal) versus 0/1 knapsack and arbitrary coin systems (greedy fails).
- `04_candyAndCookies.cpp` - Greedy distribution problems: minimum candies for rating neighbours (two passes), assigning cookies and boats carrying two people.
- `05_gasStationAndJumpGame.cpp` - Greedy scans: gas station circuit, jump game (reachability and minimum jumps) and best time to buy and sell stock.
- `06_jobSequencingWithDeadlines.cpp` - Job sequencing with deadlines: take jobs by profit into the latest free slot, with a union-find over slots for near-linear time.
- `07_meetingRoomsAndPlatforms.cpp` - Minimum meeting rooms / railway platforms: sort start and end times and sweep, or use a min-heap of end times.
- `08_intervalProblems.cpp` - Interval greedy problems: merge intervals, insert interval, minimum arrows, minimum removals for non-overlap and covering a segment.
- `09_removeKDigitsAndLargestNumber.cpp` - Greedy on digit strings: remove k digits for the smallest number (monotonic stack), largest number by concatenation order and the next greater permutation.
- `10_optimalMergePattern.cpp` - Optimal merge pattern and connecting ropes: always merge the two smallest with a min-heap (the Huffman rule).
- `11_huffmanCoding.cpp` - Huffman coding with a priority_queue: build the optimal prefix code, encode and decode, and compare with fixed-width coding.
- `12_taskSchedulerAndLoadBalancing.cpp` - Scheduling greedy: task scheduler with cooldown (formula and heap simulation), shortest-job-first waiting time and longest-processing-time load balancing.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `g++ -std=c++20 -Wall -Wextra -pthread 01_greedyActivitySelection.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/44_GreedyAlgorithms/` (git-ignored).

## Key concepts / interview angles
- **Greedy:** earliest-finish-first is optimal for interval scheduling (cut property style argument).

## Gotchas
- A greedy choice must be proven safe; coin change with 4/3/1 is the standard counterexample.
