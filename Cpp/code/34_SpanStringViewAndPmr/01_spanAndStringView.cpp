#include <array>
#include <cstdint>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

// std::span<T>: non-owning (pointer, length) view over contiguous memory -
// the C++ answer to "pass array + size" without copying or templating on the
// container. std::string_view: same idea for characters. Both are cheap to copy
// and DO NOT OWN the data: lifetime is the caller's problem.
static int sum(std::span<const int> xs)
{
    int s = 0;
    for (int x : xs)
        s += x;
    return s;
}

static void zero_fill(std::span<std::uint8_t> bytes)
{
    for (auto &b : bytes)
        b = 0;
}

// Parse "key=value" without allocating: returns views into the input.
struct KV { std::string_view key, value; };
static KV split_kv(std::string_view line)
{
    auto eq = line.find('=');
    if (eq == std::string_view::npos)
        return {line, {}};
    return {line.substr(0, eq), line.substr(eq + 1)};
}

int main()
{
    int c_array[] = {1, 2, 3, 4, 5};
    std::vector<int> vec = {10, 20, 30};
    std::array<int, 3> arr = {100, 200, 300};

    // One function accepts all contiguous containers.
    std::cout << "sum(c array) = " << sum(c_array) << '\n';
    std::cout << "sum(vector)  = " << sum(vec) << '\n';
    std::cout << "sum(array)   = " << sum(arr) << '\n';

    // Sub-views without copying; first/last/subspan.
    std::span<int> all(c_array);
    std::cout << "subspan(1,3) = " << sum(all.subspan(1, 3)) << "  (2+3+4)\n";
    std::cout << "first(2)     = " << sum(all.first(2)) << "  (1+2)\n";

    // Writable span over a byte buffer.
    std::uint8_t buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    zero_fill(std::span{buf}.subspan(2, 4));
    std::cout << "buf after zero_fill(2..5):";
    for (auto b : buf)
        std::cout << ' ' << int(b);
    std::cout << '\n';

    // Static extent: size is part of the type -> checked at compile time.
    std::span<int, 3> fixed(arr);
    std::cout << "fixed extent size = " << fixed.size() << '\n';

    // string_view: zero-copy parsing.
    std::string config = "timeout=30";
    auto kv = split_kv(config);
    std::cout << "key=\"" << kv.key << "\" value=\"" << kv.value << "\"\n";

    // string_view from a literal / substring: no allocation at all.
    std::string_view sv = "hello world";
    std::cout << "sv.substr(6) = " << sv.substr(6) << '\n';

    // ---- Lifetime traps (do NOT do these) ----
    //  std::string_view bad = std::string("temporary");   // dangling after the ;
    //  std::span<int> bad2 = std::vector<int>{1,2,3};     // (const span won't even compile for rvalue-owning)
    //  v.push_back(...) after taking a span of v           // may reallocate -> span dangles
    //  string_view is NOT null-terminated: passing sv.data() to a C API expecting
    //  a C string can read past the end - copy to std::string first.
    std::vector<int> v = {1, 2, 3};
    std::span<int> s(v);
    v.reserve(1000);          // reallocation likely: 's' now dangles. Re-create it.
    s = v;
    std::cout << "re-created span ok, size=" << s.size() << '\n';
}
