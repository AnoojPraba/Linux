// errno -> std::system_error, std::errc comparison, sigaction with a lock-free
// atomic flag.

#include <atomic>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <iostream>
#include <string>
#include <system_error>

std::atomic<int> last_signal{0};                       // lock-free atomics are allowed in handlers

extern "C" void on_signal(int signo)
{
    last_signal.store(signo, std::memory_order_relaxed);   // set a flag and return: nothing else is safe
}

std::string read_first_line(const char *path)
{
    std::FILE *fp = std::fopen(path, "r");
    if (!fp)
        throw std::system_error(errno, std::generic_category(), path);   // errno becomes an exception
    char buf[128] = {};
    if (!std::fgets(buf, sizeof buf, fp))
        buf[0] = '\0';
    std::fclose(fp);
    return buf;
}

int main()
{
    try
    {
        read_first_line("/no/such/file");
    }
    catch (const std::system_error &e)
    {
        std::cout << "code " << e.code().value() << " (" << e.code().message() << "), ENOENT: "
                  << std::boolalpha << (e.code() == std::errc::no_such_file_or_directory) << '\n';
    }

    struct sigaction sa{};
    sa.sa_handler = on_signal;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, nullptr);                  // sigaction has well-defined semantics, signal() does not

    std::raise(SIGUSR1);
    std::cout << "handler saw signal " << last_signal.load() << " (SIGUSR1 = " << SIGUSR1 << ")\n";
    std::cout << "is_always_lock_free: " << std::atomic<int>::is_always_lock_free << '\n';
}
