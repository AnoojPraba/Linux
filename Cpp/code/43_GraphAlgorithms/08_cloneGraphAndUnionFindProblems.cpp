// Common graph interview problems: clone a graph, redundant connection and
// accounts merging with union-find, and finding the town judge by degrees.

#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

struct Node
{
    int val;
    std::vector<Node *> neighbours;
};

Node *clone_graph(Node *start, std::vector<std::unique_ptr<Node>> &pool)
{
    std::unordered_map<Node *, Node *> copy;                              // old node -> new node, doubles as the visited set
    std::queue<Node *> q;
    auto make = [&](Node *old) { pool.push_back(std::make_unique<Node>(Node{old->val, {}})); return copy[old] = pool.back().get(); };
    make(start);
    q.push(start);
    while (!q.empty())
    {
        Node *u = q.front(); q.pop();
        for (Node *v : u->neighbours)
        {
            if (!copy.count(v)) { make(v); q.push(v); }
            copy[u]->neighbours.push_back(copy[v]);
        }
    }
    return copy[start];
}

struct Dsu
{
    std::vector<int> p;
    explicit Dsu(int n) : p(n) { std::iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) { a = find(a); b = find(b); if (a == b) return false; p[a] = b; return true; }
};

int main()
{
    std::vector<std::unique_ptr<Node>> original, copied;
    for (int i = 1; i <= 4; ++i) original.push_back(std::make_unique<Node>(Node{i, {}}));
    auto link = [&](int a, int b) { original[a]->neighbours.push_back(original[b].get()); original[b]->neighbours.push_back(original[a].get()); };
    link(0, 1); link(1, 2); link(2, 3); link(3, 0);
    Node *c = clone_graph(original[0].get(), copied);
    std::cout << "clone is a different object: " << std::boolalpha << (c != original[0].get()) << ", copied " << copied.size() << " nodes, neighbours of the clone of 1:";
    for (Node *v : c->neighbours) std::cout << ' ' << v->val;

    std::vector<std::pair<int, int>> edges{{1, 2}, {1, 3}, {2, 3}, {3, 4}, {1, 4}};          // the last edge that closes a cycle
    Dsu d(5);
    std::pair<int, int> redundant{-1, -1};
    for (auto e : edges) if (!d.unite(e.first, e.second)) redundant = e;
    std::cout << "\nredundant connection (last edge that forms a cycle): " << redundant.first << '-' << redundant.second;

    std::vector<std::vector<std::string>> accounts{{"John", "a@x", "b@x"}, {"John", "b@x", "c@x"}, {"Mary", "m@x"}, {"John", "z@x"}};
    Dsu acc(static_cast<int>(accounts.size()));
    std::map<std::string, int> owner;                                                          // an email address links accounts
    for (std::size_t i = 0; i < accounts.size(); ++i)
        for (std::size_t j = 1; j < accounts[i].size(); ++j)
        {
            auto [it, fresh] = owner.emplace(accounts[i][j], static_cast<int>(i));
            if (!fresh) acc.unite(static_cast<int>(i), it->second);
        }
    std::map<int, std::set<std::string>> merged;
    for (auto &[email, i] : owner) merged[acc.find(i)].insert(email);
    std::cout << "\nmerged accounts: " << merged.size() << " (John's first two share b@x)";

    int n = 4;                                                                                  // town judge: trusted by everyone (in-degree n-1), trusts nobody
    std::vector<int> in(n + 1, 0), out(n + 1, 0);
    for (auto [a, b] : std::vector<std::pair<int, int>>{{1, 3}, {2, 3}, {4, 3}}) { ++out[a]; ++in[b]; }
    for (int v = 1; v <= n; ++v) if (in[v] == n - 1 && out[v] == 0) std::cout << "\ntown judge: " << v;
    std::cout << '\n';
}
