// Scheduling greedy: task scheduler with cooldown (formula and heap
// simulation), shortest-job-first waiting time and longest-processing-time
// load balancing.

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <vector>

int cooldown_time(const std::string &tasks, int n)
{
    std::vector<int> freq(26, 0);
    for (char c : tasks) ++freq[c - 'A'];
    int mx = *std::max_element(freq.begin(), freq.end()), num_max = static_cast<int>(std::count(freq.begin(), freq.end(), mx));
    return std::max(static_cast<int>(tasks.size()), (mx - 1) * (n + 1) + num_max);   // (max-1) full frames, then the last row of max-count tasks
}

double average_wait_sjf(std::vector<int> bursts)
{
    std::sort(bursts.begin(), bursts.end());                            // shortest job first minimizes the average waiting time
    long wait = 0, elapsed = 0;
    for (int b : bursts) { wait += elapsed; elapsed += b; }
    return static_cast<double>(wait) / bursts.size();
}

int lpt_makespan(std::vector<int> jobs, int machines)                   // longest processing time first onto the least loaded machine
{
    std::sort(jobs.begin(), jobs.end(), std::greater<>());
    std::priority_queue<int, std::vector<int>, std::greater<>> load;
    for (int i = 0; i < machines; ++i) load.push(0);
    for (int j : jobs) { int l = load.top(); load.pop(); load.push(l + j); }
    int mx = 0;
    while (!load.empty()) { mx = std::max(mx, load.top()); load.pop(); }
    return mx;
}

int main()
{
    std::cout << "AAABBBCCD with cooldown 2: " << cooldown_time("AAABBBCCD", 2) << ", AAAAAABCDEFG cooldown 2: " << cooldown_time("AAAAAABCDEFG", 2) << '\n';
    std::cout << "average waiting time, bursts 6 8 7 3: SJF " << average_wait_sjf({6, 8, 7, 3}) << " vs given order " << (0 + 6 + 14 + 21) / 4.0 << '\n';
    std::cout << "LPT makespan for jobs 7 5 4 4 3 3 on 3 machines: " << lpt_makespan({7, 5, 4, 4, 3, 3}, 3) << " (a 4/3-approximation of the optimum)\n";
}
