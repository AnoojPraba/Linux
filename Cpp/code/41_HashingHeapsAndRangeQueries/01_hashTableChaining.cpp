// vector of std::list buckets with std::hash; the idea behind unordered_map.

#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <vector>

class HashTable
{
    std::vector<std::list<std::pair<std::string, int>>> buckets;

    std::size_t index(const std::string &key) const { return std::hash<std::string>{}(key) % buckets.size(); }

public:
    explicit HashTable(std::size_t n = 8) : buckets(n) {}

    void add(const std::string &key)                  // count one more occurrence of key
    {
        for (auto &kv : buckets[index(key)])
            if (kv.first == key)
            {
                ++kv.second;
                return;
            }
        buckets[index(key)].emplace_back(key, 1);
    }

    int get(const std::string &key) const
    {
        for (const auto &kv : buckets[index(key)])
            if (kv.first == key)
                return kv.second;
        return 0;
    }
};

int main()
{
    HashTable t;
    for (const char *w : {"to", "be", "or", "not", "to", "be", "that", "is", "the", "question", "to"})
        t.add(w);
    std::cout << "to=" << t.get("to") << " be=" << t.get("be") << " or=" << t.get("or")
              << " question=" << t.get("question") << " missing=" << t.get("zebra") << '\n';
}
