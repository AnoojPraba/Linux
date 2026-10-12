// std::sort versus stable_sort, strict weak ordering rules, multi-key
// comparators with tuples and C++20 ranges::sort with projections.

#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

struct Student
{
    std::string name;
    int grade;
    int age;
};

int main()
{
    std::vector<Student> s{{"ann", 2, 20}, {"raj", 1, 22}, {"li", 2, 19}, {"sam", 1, 22}, {"zoe", 2, 20}};
    auto print = [](const char *label, const std::vector<Student> &v) {
        std::cout << label;
        for (auto &x : v) std::cout << ' ' << x.name << '(' << x.grade << ')';
        std::cout << '\n';
    };

    auto a = s;
    std::stable_sort(a.begin(), a.end(), [](const Student &x, const Student &y) { return x.grade < y.grade; });
    print("stable_sort by grade        :", a);                          // equal grades keep the input order

    auto b = s;
    std::sort(b.begin(), b.end(), [](const Student &x, const Student &y) {
        return std::tie(x.grade, x.age, x.name) < std::tie(y.grade, y.age, y.name);    // lexicographic multi-key comparison
    });
    print("sort by grade, age, name    :", b);

    auto c = s;
    std::ranges::sort(c, std::greater<>{}, &Student::age);                // C++20: sort by a projection
    std::cout << "ranges::sort by age desc  :";
    for (auto &x : c) std::cout << ' ' << x.age;

    auto d = s;
    std::ranges::stable_sort(d, {}, &Student::name);                      // by name with the default ordering
    print("\nranges::stable_sort by name :", d);

    std::cout << "comparator rules: strict (comp(a, a) is false), irreflexive/asymmetric/transitive; <= breaks them and can crash std::sort\n";
    std::cout << "is_sorted by name: " << std::boolalpha << std::ranges::is_sorted(d, {}, &Student::name) << '\n';
}
