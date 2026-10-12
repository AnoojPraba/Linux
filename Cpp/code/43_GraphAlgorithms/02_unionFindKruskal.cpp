// Disjoint sets (union by size + path halving) and Kruskal's minimum spanning
// tree.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

class DisjointSets
{
    std::vector<int> parent_, size_;
    int components_;

public:
    explicit DisjointSets(int n) : parent_(n), size_(n, 1), components_(n) { std::iota(parent_.begin(), parent_.end(), 0); }

    int find(int x)
    {
        while (parent_[x] != x)
        {
            parent_[x] = parent_[parent_[x]];            // path halving: flatten the tree as we climb
            x = parent_[x];
        }
        return x;
    }

    bool unite(int a, int b)
    {
        a = find(a);
        b = find(b);
        if (a == b) return false;                        // already in the same set
        if (size_[a] < size_[b]) std::swap(a, b);
        parent_[b] = a;                                  // attach the smaller tree under the larger
        size_[a] += size_[b];
        --components_;
        return true;
    }

    int components() const { return components_; }
};

struct Edge { int u, v, w; };

int main()
{
    std::vector<Edge> edges{{0, 1, 7}, {0, 3, 5}, {1, 2, 8}, {1, 3, 9}, {1, 4, 7}, {2, 4, 5},
                            {3, 4, 15}, {3, 5, 6}, {4, 5, 8}, {4, 6, 9}, {5, 6, 11}};
    std::sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) { return a.w < b.w; });   // cheapest first

    DisjointSets ds(7);
    int total = 0, used = 0;
    for (const Edge &e : edges)
        if (ds.unite(e.u, e.v))                           // skip an edge that would close a cycle
        {
            std::cout << "take " << e.u << '-' << e.v << " (" << e.w << ")\n";
            total += e.w;
            ++used;
        }
    std::cout << "MST edges=" << used << " total weight=" << total << " components=" << ds.components() << '\n';
}
