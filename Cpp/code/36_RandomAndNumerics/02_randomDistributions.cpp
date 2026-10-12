// mt19937/mt19937_64 first outputs, uniform/normal/bernoulli distributions
// checked by properties, seed_seq seeding.

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <vector>

int main()
{
    std::mt19937 gen(5489);                                    // the engine: a deterministic source of 32-bit values
    std::cout << "mt19937 first output (specified by the standard): " << gen() << '\n';
    std::mt19937_64 gen64(5489);
    std::cout << "mt19937_64 first output: " << gen64() << '\n';

    // Distributions turn raw bits into the numbers you want. Their exact output is NOT specified, so test properties.
    std::mt19937 rng(7);
    std::uniform_int_distribution<int> die(1, 6);
    std::map<int, int> hist;
    for (int i = 0; i < 60000; ++i) ++hist[die(rng)];
    bool balanced = std::all_of(hist.begin(), hist.end(), [](auto &kv) { return kv.second > 9500 && kv.second < 10500; });
    std::cout << "die faces: " << hist.size() << ", each about 10000: " << std::boolalpha << balanced << '\n';

    std::normal_distribution<double> bell(100.0, 15.0);       // mean 100, standard deviation 15
    double sum = 0;
    for (int i = 0; i < 100000; ++i) sum += bell(rng);
    std::cout << "mean of 100000 normal samples is near 100: " << (std::abs(sum / 100000 - 100.0) < 0.5) << '\n';

    std::bernoulli_distribution coin(0.25);                    // true with probability 0.25
    int heads = 0;
    for (int i = 0; i < 40000; ++i) heads += coin(rng);
    std::cout << "P(true) near 0.25: " << (std::abs(heads / 40000.0 - 0.25) < 0.02) << '\n';

    std::vector<int> deck(10);
    std::iota(deck.begin(), deck.end(), 0);
    std::shuffle(deck.begin(), deck.end(), rng);               // uniformly random permutation (Fisher-Yates)
    std::cout << "shuffled deck is a permutation: " << std::is_permutation(deck.begin(), deck.end(), std::vector<int>{0,1,2,3,4,5,6,7,8,9}.begin()) << '\n';

    // seeding properly: a different sequence each run
    std::random_device rd;
    std::seed_seq seq{rd(), rd(), rd(), rd()};                 // mixes several words of entropy into the engine state
    std::mt19937 fresh(seq);
    (void)fresh;
}
