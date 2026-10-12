// srand/rand versus mt19937 + uniform_int/real_distribution, shuffle,
// random_device.

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main()
{
    std::srand(42);                                    // C style: seed once, fixed seed => same sequence every run
    int r1 = std::rand(), r2 = std::rand();
    std::srand(42);
    std::cout << "srand(42) repeats the same sequence: " << std::boolalpha << (std::rand() == r1 && std::rand() == r2) << '\n';

    std::mt19937 gen(5489);                            // the Mersenne Twister engine with its default seed
    std::cout << "first output of mt19937(5489): " << gen() << '\n';

    std::mt19937 dice_gen(2024);
    std::uniform_int_distribution<int> dice(1, 6);     // an integer in [1, 6], each value equally likely
    int counts[7] = {0};
    bool in_range = true;
    for (int i = 0; i < 6000; ++i)
    {
        int d = dice(dice_gen);
        in_range = in_range && d >= 1 && d <= 6;
        ++counts[d];
    }
    std::cout << "6000 rolls, all between 1 and 6: " << in_range << ", every face seen: "
              << std::all_of(counts + 1, counts + 7, [](int c) { return c > 500; }) << '\n';

    std::uniform_real_distribution<double> unit(0.0, 1.0);        // a real number in [0, 1)
    double u = unit(dice_gen);
    std::cout << "a real number in [0, 1): " << (u >= 0.0 && u < 1.0) << '\n';

    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
    std::shuffle(v.begin(), v.end(), std::mt19937(1));             // shuffle needs a generator, not rand()
    std::cout << "shuffled sum is still " << std::accumulate(v.begin(), v.end(), 0) << '\n';

    std::random_device rd;                                         // non-deterministic seed source (hardware or the OS)
    std::mt19937 seeded(rd());                                     // use this pattern to get a different sequence each run
    (void)seeded;
}
