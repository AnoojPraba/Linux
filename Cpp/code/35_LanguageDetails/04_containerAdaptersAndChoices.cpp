// stack/queue/priority_queue over chosen containers, list::splice, transparent
// set lookup, deque reference stability, forward_list.

#include <deque>
#include <forward_list>
#include <functional>
#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

int main()
{
    // Adapters wrap a sequence container and expose a restricted interface.
    std::stack<int, std::vector<int>> s;                 // the default container is deque; vector works too
    s.push(1); s.push(2);
    std::queue<int> q;                                   // needs push_back/pop_front: deque (default) or list, not vector
    q.push(1); q.push(2);
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;   // heap in a vector, smallest on top
    for (int x : {5, 1, 4}) pq.push(x);
    std::cout << "stack top " << s.top() << ", queue front " << q.front() << ", priority_queue top " << pq.top() << '\n';

    // list: O(1) splice moves nodes between lists without copying or invalidating iterators.
    std::list<std::string> a{"x", "y"}, b{"p", "q", "r"};
    auto it = b.begin();
    ++it;                                                // points at "q"
    a.splice(a.end(), b, it);                            // the node itself moves
    std::cout << "list a:";
    for (auto &t : a) std::cout << ' ' << t;
    std::cout << ", the iterator still points at: " << *it << ", b has " << b.size() << " elements\n";

    // set with a transparent comparator: look up a std::string_view without building a std::string
    std::set<std::string, std::less<>> names{"alice", "bob", "carol"};
    std::cout << "heterogeneous lookup of \"bob\": " << (names.find("bob") != names.end()) << '\n';

    // deque: stable references on push at both ends, but not contiguous
    std::deque<int> d{1, 2, 3};
    int &ref = d[1];
    d.push_front(0);
    d.push_back(4);
    std::cout << "deque reference survived push_front/push_back: " << ref << '\n';

    std::vector<int> v{1, 2, 3};
    int &vref = v[1];
    v.reserve(1000);                                     // may reallocate: vref would now dangle
    (void)vref;
    std::forward_list<int> fl{3, 1, 2};
    fl.sort();
    fl.insert_after(fl.before_begin(), 0);               // a singly linked list inserts AFTER a position
    std::cout << "forward_list:";
    for (int x : fl) std::cout << ' ' << x;
    std::cout << '\n';
}
