// Interval scheduling: sort by finish time, take what fits.

#include <algorithm>
#include <iostream>
#include <vector>

struct Activity
{
    int start, finish;
};

int main()
{
    std::vector<Activity> act{{1, 4}, {3, 5}, {0, 6}, {5, 7}, {3, 9}, {5, 9}, {6, 10}, {8, 11}, {8, 12}, {2, 14}, {12, 16}};
    std::sort(act.begin(), act.end(), [](const Activity &a, const Activity &b) { return a.finish < b.finish; });

    int last_finish = -1, chosen = 0;
    for (const auto &a : act)
        if (a.start >= last_finish)                  // does not overlap the last chosen activity
        {
            std::cout << "take [" << a.start << ", " << a.finish << "]\n";
            last_finish = a.finish;
            ++chosen;
        }
    std::cout << chosen << " activities\n";
}
