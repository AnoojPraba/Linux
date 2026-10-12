// steady vs system clock, duration literals and casts, sleep_for/sleep_until.

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <thread>

int main()
{
    using namespace std::chrono;
    using namespace std::chrono_literals;

    // A calendar time: fixed here so the output is repeatable.
    system_clock::time_point tp = system_clock::from_time_t(1700000000);
    std::time_t t = system_clock::to_time_t(tp + 36h);
    std::tm tm = *std::gmtime(&t);
    std::cout << "36 h after the given time: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << '\n';

    // Durations are typed and convert implicitly only when no precision is lost.
    seconds s = 2min + 30s;
    milliseconds ms = s;                               // seconds -> milliseconds: exact, implicit
    auto whole = duration_cast<minutes>(s);            // minutes: truncates, so the cast is explicit
    std::cout << s.count() << " s = " << ms.count() << " ms = " << whole.count() << " whole minutes\n";
    std::cout << "ratio of 90 min to 1 h: " << duration<double, std::ratio<3600>>(90min).count() << " h\n";

    // Measuring elapsed time: use a monotonic clock.
    static_assert(steady_clock::is_steady);
    auto start = steady_clock::now();
    std::this_thread::sleep_for(20ms);
    std::this_thread::sleep_until(start + 60ms);       // sleep until an absolute point in time
    auto elapsed = duration_cast<milliseconds>(steady_clock::now() - start);
    std::cout << "elapsed >= 60 ms: " << std::boolalpha << (elapsed >= 60ms) << '\n';
    std::cout << "system_clock steady: " << system_clock::is_steady << '\n';
}
