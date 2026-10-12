// Job sequencing with deadlines: take jobs by profit into the latest free
// slot, with a union-find over slots for near-linear time.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Job { char id; int deadline, profit; };

int main()
{
    std::vector<Job> jobs{{'a', 2, 100}, {'b', 1, 19}, {'c', 2, 27}, {'d', 1, 25}, {'e', 3, 15}};
    std::sort(jobs.begin(), jobs.end(), [](const Job &x, const Job &y) { return x.profit > y.profit; });

    int max_deadline = 3;
    std::vector<int> parent(max_deadline + 1);                         // parent[t] = latest free slot <= t (0 = none)
    std::iota(parent.begin(), parent.end(), 0);
    auto find = [&](int t) { while (parent[t] != t) t = parent[t] = parent[parent[t]]; return t; };

    std::string schedule(max_deadline + 1, '-');
    int total = 0;
    for (const Job &j : jobs)
    {
        int slot = find(std::min(j.deadline, max_deadline));
        if (slot == 0) continue;                                       // no free slot before the deadline: skip the job
        schedule[slot] = j.id;
        total += j.profit;
        parent[slot] = slot - 1;                                       // the slot is taken: link it to the previous one
    }
    std::cout << "slots 1..3: " << schedule.substr(1) << ", total profit " << total << '\n';
    std::cout << "sort by profit, put each job in the latest free slot <= its deadline; union-find finds that slot in near-O(1)\n";
}
