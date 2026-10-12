// Cocktail shaker, comb and gnome sort: simple sorts that improve on bubble
// sort in different ways.

#include <iostream>
#include <utility>
#include <vector>

void cocktail(std::vector<int> &a)
{
    std::size_t lo = 0, hi = a.size();
    bool swapped = true;
    while (swapped && hi - lo > 1)
    {
        swapped = false;
        for (std::size_t i = lo; i + 1 < hi; ++i) if (a[i] > a[i + 1]) { std::swap(a[i], a[i + 1]); swapped = true; }
        --hi;
        for (std::size_t i = hi - 1; i > lo; --i) if (a[i - 1] > a[i]) { std::swap(a[i - 1], a[i]); swapped = true; }
        ++lo;
    }
}

void comb(std::vector<int> &a)
{
    std::size_t gap = a.size();
    bool swapped = true;
    while (gap > 1 || swapped)
    {
        gap = std::max<std::size_t>(1, gap * 10 / 13);     // shrink factor about 1.3
        swapped = false;
        for (std::size_t i = 0; i + gap < a.size(); ++i)
            if (a[i] > a[i + gap]) { std::swap(a[i], a[i + gap]); swapped = true; }
    }
}

void gnome(std::vector<int> &a)
{
    for (std::size_t i = 0; i < a.size();)
        if (i == 0 || a[i - 1] <= a[i]) ++i;
        else { std::swap(a[i], a[i - 1]); --i; }
}

int main()
{
    const std::vector<int> base{5, 1, 4, 2, 8, 0, 2, 9, -3};
    auto show = [](const char *name, const std::vector<int> &v) { std::cout << name << ':'; for (int x : v) std::cout << ' ' << x; std::cout << '\n'; };
    auto a = base; cocktail(a); show("cocktail", a);
    auto b = base; comb(b);     show("comb    ", b);
    auto c = base; gnome(c);    show("gnome   ", c);
}
