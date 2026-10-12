// if/switch with initializer, structured bindings, [[fallthrough]], lock_guard
// scoped to an if.

#include <iostream>
#include <map>
#include <mutex>
#include <string>

enum class Op { Add, Sub, Other };

Op parse(char c)
{
    switch (c) { case '+': return Op::Add; case '-': return Op::Sub; default: return Op::Other; }
}

int main()
{
    std::map<std::string, int> stock{{"apple", 5}, {"pear", 0}};

    // if with an initializer: 'it' exists only inside the if/else, so it cannot leak or be reused by mistake.
    if (auto it = stock.find("apple"); it != stock.end())
        std::cout << "apple: " << it->second << '\n';
    else
        std::cout << "no apple\n";

    // structured binding takes a pair, tuple or struct apart.
    for (const auto &[name, count] : stock)
        std::cout << name << " -> " << count << '\n';

    // insert returns {iterator, bool}: bind both with a structured binding in an if with an initializer.
    if (auto [pos, inserted] = stock.insert({"plum", 3}); inserted)
        std::cout << "inserted " << pos->first << '\n';

    // the lock is held only for the if statement, a common use.
    std::mutex m;
    int shared = 10;
    if (std::lock_guard lock(m); shared > 5)
        std::cout << "shared is large\n";

    // switch with an initializer (C++17) and [[likely]] / [[fallthrough]] attributes (C++20 / C++17)
    switch (Op op = parse('+'); op)
    {
    case Op::Add:
        std::cout << "add\n";
        [[fallthrough]];
    case Op::Sub:
        std::cout << "add or sub path\n";
        break;
    default:
        break;
    }
}
