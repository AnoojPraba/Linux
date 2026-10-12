// Linear probing, tombstones, load-factor 0.7 resizing, heterogeneous
// operations.

#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

template <typename K, typename V, typename Hash = std::hash<K>>
class OpenTable
{
    enum class State : unsigned char { Empty, Full, Deleted };
    struct Slot
    {
        State state = State::Empty;
        K key{};
        V value{};
    };
    std::vector<Slot> slots_;
    std::size_t used_ = 0;        // Full + Deleted: both lengthen probe sequences
    std::size_t live_ = 0;
    Hash hash_;

    void grow()
    {
        std::vector<Slot> old = std::move(slots_);
        slots_.assign(old.size() * 2, Slot{});
        used_ = live_ = 0;
        for (auto &s : old)
            if (s.state == State::Full)
                insert(std::move(s.key), std::move(s.value));
    }

public:
    explicit OpenTable(std::size_t n = 8) : slots_(n) {}

    void insert(K key, V value)
    {
        if ((used_ + 1) * 10 > slots_.size() * 7) grow();                 // keep the load factor below 0.7
        std::size_t i = hash_(key) % slots_.size();
        std::optional<std::size_t> tomb;
        while (slots_[i].state != State::Empty)
        {
            if (slots_[i].state == State::Full && slots_[i].key == key) { slots_[i].value = std::move(value); return; }
            if (slots_[i].state == State::Deleted && !tomb) tomb = i;     // remember the first reusable slot
            i = (i + 1) % slots_.size();                                  // linear probing
        }
        if (tomb) i = *tomb; else ++used_;
        slots_[i] = Slot{State::Full, std::move(key), std::move(value)};
        ++live_;
    }

    const V *find(const K &key) const
    {
        std::size_t i = hash_(key) % slots_.size();
        while (slots_[i].state != State::Empty)                           // a tombstone does not end the search
        {
            if (slots_[i].state == State::Full && slots_[i].key == key) return &slots_[i].value;
            i = (i + 1) % slots_.size();
        }
        return nullptr;
    }

    bool erase(const K &key)
    {
        std::size_t i = hash_(key) % slots_.size();
        while (slots_[i].state != State::Empty)
        {
            if (slots_[i].state == State::Full && slots_[i].key == key)
            {
                slots_[i].state = State::Deleted;                         // a tombstone keeps later probes working
                --live_;
                return true;
            }
            i = (i + 1) % slots_.size();
        }
        return false;
    }

    std::size_t size() const { return live_; }
    std::size_t capacity() const { return slots_.size(); }
};

int main()
{
    OpenTable<std::string, int> t;
    for (int i = 0; i < 100; ++i)
        t.insert("key" + std::to_string(i), i * i);
    std::cout << "size=" << t.size() << " capacity=" << t.capacity() << '\n';

    std::cout << "key42 -> " << *t.find("key42") << '\n';
    std::cout << "erase key42: " << std::boolalpha << t.erase("key42") << ", find again: " << (t.find("key42") != nullptr) << '\n';
    std::cout << "key43 still found after the erase: " << (t.find("key43") != nullptr) << '\n';
    t.insert("key42", -1);
    std::cout << "reinserted key42 -> " << *t.find("key42") << ", size=" << t.size() << '\n';
    t.insert("key7", 777);                                               // overwrite an existing key
    std::cout << "key7 -> " << *t.find("key7") << ", size=" << t.size() << '\n';
}
