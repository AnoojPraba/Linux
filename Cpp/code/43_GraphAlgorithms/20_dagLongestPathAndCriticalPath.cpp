// DP on a DAG: longest path, earliest start times and the critical path of a
// project schedule, plus counting paths.

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

int main()
{
    std::vector<std::string> name{"design", "backend", "frontend", "database", "integrate", "test", "release"};
    std::vector<int> dur{3, 5, 4, 2, 2, 3, 1};
    std::vector<std::vector<int>> next(7);
    next[0] = {1, 2, 3}; next[1] = {4}; next[2] = {4}; next[3] = {4}; next[4] = {5}; next[5] = {6};

    std::vector<int> indeg(7, 0), order;
    for (auto &adj : next) for (int v : adj) ++indeg[v];
    std::queue<int> q;
    for (int i = 0; i < 7; ++i) if (!indeg[i]) q.push(i);
    while (!q.empty()) { int u = q.front(); q.pop(); order.push_back(u); for (int v : next[u]) if (--indeg[v] == 0) q.push(v); }

    std::vector<int> start(7, 0), pred(7, -1);
    for (int u : order)                                                      // a task starts when its slowest prerequisite ends
        for (int v : next[u]) if (start[u] + dur[u] > start[v]) { start[v] = start[u] + dur[u]; pred[v] = u; }
    int last = static_cast<int>(std::max_element(order.begin(), order.end(), [&](int a, int b) { return start[a] + dur[a] < start[b] + dur[b]; }) - order.begin());
    last = order[last];
    for (int i = 0; i < 7; ++i) std::cout << "  " << name[i] << " starts at " << start[i] << ", ends at " << start[i] + dur[i] << '\n';
    std::vector<int> chain;
    for (int v = last; v >= 0; v = pred[v]) chain.push_back(v);
    std::reverse(chain.begin(), chain.end());
    std::cout << "project length " << start[last] + dur[last] << ", critical path:";
    for (int v : chain) std::cout << ' ' << name[v];

    std::vector<long> paths(7, 0);                                           // number of distinct paths from 0 to each node
    paths[0] = 1;
    for (int u : order) for (int v : next[u]) paths[v] += paths[u];
    std::cout << "\ndistinct paths from design to release: " << paths[6] << '\n';
    std::cout << "process nodes in topological order and relax outgoing edges: O(V + E); works with negative weights too (no cycles)\n";
}
