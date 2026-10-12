// 2-SAT with strongly connected components: build the implication graph, test
// satisfiability and produce an assignment.

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

class TwoSat
{
    int n_;
    std::vector<std::vector<int>> g_;
    static int node(int var, bool value) { return 2 * var + (value ? 0 : 1); }   // node 2i = x_i true, 2i+1 = x_i false

public:
    explicit TwoSat(int n) : n_(n), g_(2 * n) {}
    void add_clause(int a, bool va, int b, bool vb)                              // (a == va) or (b == vb)
    {
        g_[node(a, !va)].push_back(node(b, vb));                                  // NOT a -> b
        g_[node(b, !vb)].push_back(node(a, va));                                  // NOT b -> a
    }
    bool solve(std::vector<bool> &assignment)
    {
        int m = 2 * n_, timer = 0, comps = 0;
        std::vector<int> idx(m, 0), low(m, 0), comp(m, -1), stack;
        std::vector<bool> on(m, false);
        std::function<void(int)> dfs = [&](int u) {
            idx[u] = low[u] = ++timer; stack.push_back(u); on[u] = true;
            for (int v : g_[u]) { if (!idx[v]) { dfs(v); low[u] = std::min(low[u], low[v]); } else if (on[v]) low[u] = std::min(low[u], idx[v]); }
            if (low[u] == idx[u]) { for (int v = -1; v != u;) { v = stack.back(); stack.pop_back(); on[v] = false; comp[v] = comps; } ++comps; }
        };
        for (int i = 0; i < m; ++i) if (!idx[i]) dfs(i);
        assignment.assign(n_, false);
        for (int i = 0; i < n_; ++i)
        {
            if (comp[2 * i] == comp[2 * i + 1]) return false;                     // x and NOT x in the same SCC: contradiction
            assignment[i] = comp[2 * i] < comp[2 * i + 1];                        // Tarjan ids are in reverse topological order: take the later component
        }
        return true;
    }
};

int main()
{
    TwoSat s(4);
    s.add_clause(0, true, 1, true);                                              // x0 or x1
    s.add_clause(0, false, 2, true);                                             // not x0 or x2
    s.add_clause(1, false, 2, false);                                            // not x1 or not x2
    s.add_clause(2, true, 3, true);                                              // x2 or x3
    s.add_clause(3, false, 0, true);                                             // not x3 or x0
    std::vector<bool> a;
    bool ok = s.solve(a);
    std::cout << "satisfiable: " << std::boolalpha << ok << '\n';
    if (ok) for (int i = 0; i < 4; ++i) std::cout << "  x" << i << " = " << a[i] << '\n';

    TwoSat bad(1);
    bad.add_clause(0, true, 0, true);                                            // x0
    bad.add_clause(0, false, 0, false);                                          // not x0
    std::cout << "x0 and not x0: satisfiable = " << bad.solve(a) << '\n';
    std::cout << "linear time for 2-SAT; 3-SAT is NP-complete\n";
}
