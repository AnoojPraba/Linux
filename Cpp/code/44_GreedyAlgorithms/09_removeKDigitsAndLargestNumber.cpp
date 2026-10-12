// Greedy on digit strings: remove k digits for the smallest number (monotonic
// stack), largest number by concatenation order and the next greater
// permutation.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::string remove_k_digits(const std::string &num, int k)
{
    std::string st;
    for (char c : num)
    {
        while (k > 0 && !st.empty() && st.back() > c) { st.pop_back(); --k; }     // a larger digit before a smaller one: dropping it helps
        st += c;
    }
    st.resize(st.size() - k);                                           // remaining removals come off the end
    std::size_t start = st.find_first_not_of('0');
    return start == std::string::npos ? "0" : st.substr(start);
}

std::string largest_number(std::vector<int> nums)
{
    std::sort(nums.begin(), nums.end(), [](int a, int b) { return std::to_string(a) + std::to_string(b) > std::to_string(b) + std::to_string(a); });   // concatenation order
    std::string out;
    for (int n : nums) out += std::to_string(n);
    return out[0] == '0' ? "0" : out;
}

int main()
{
    std::cout << "remove 3 digits from 1432219: " << remove_k_digits("1432219", 3) << ", 1 from 10200: " << remove_k_digits("10200", 1) << ", 2 from 10: " << remove_k_digits("10", 2) << '\n';
    std::cout << "largest number from 3 30 34 5 9: " << largest_number({3, 30, 34, 5, 9}) << '\n';
    std::string n = "6543";
    std::cout << "next greater permutation of 1243 is ";
    std::string m = "1243";
    std::next_permutation(m.begin(), m.end());
    std::cout << m << ", of 6543 exists: " << std::boolalpha << std::next_permutation(n.begin(), n.end()) << '\n';
}
