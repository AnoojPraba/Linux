// Detect input..contiguous categories with concepts, std::distance/advance
// cost, invalidation.

#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

template <std::input_iterator It>
void describe(const char *name, It)
{
    std::cout << name << ": ";
    if constexpr (std::contiguous_iterator<It>)            std::cout << "contiguous";
    else if constexpr (std::random_access_iterator<It>)    std::cout << "random access";
    else if constexpr (std::bidirectional_iterator<It>)    std::cout << "bidirectional";
    else if constexpr (std::forward_iterator<It>)          std::cout << "forward";
    else                                                   std::cout << "input only";
    std::cout << '\n';
}

// std::distance and std::advance are O(1) on random access iterators and O(n) otherwise.
template <std::input_iterator It>
auto middle(It first, It last)
{
    auto n = std::distance(first, last);                  // chooses the best implementation for the category
    std::advance(first, n / 2);
    return *first;
}

int main()
{
    std::vector<int> v{1, 2, 3, 4, 5};
    std::list<int> l{1, 2, 3, 4, 5};
    std::forward_list<int> f{1, 2, 3, 4, 5};
    describe("vector", v.begin());
    describe("list", l.begin());
    describe("forward_list", f.begin());
    describe("istream_iterator", std::istream_iterator<int>());
    describe("int* (raw pointer)", v.data());

    std::cout << "middle of vector " << middle(v.begin(), v.end()) << ", of list " << middle(l.begin(), l.end()) << '\n';

    auto it = v.begin();
    v.push_back(6);                                       // may reallocate: every iterator into v is now invalid
    (void)it;
    std::cout << "list iterators survive insertion: ";
    auto lit = l.begin();
    l.push_front(0);
    std::cout << *lit << '\n';
}
