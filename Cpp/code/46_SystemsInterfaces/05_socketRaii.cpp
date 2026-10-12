// Move-only RAII file-descriptor wrapper; TCP server and client (client in a
// thread) over loopback.

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <thread>
#include <utility>

class Fd                                               // owns a file descriptor: closes it exactly once
{
    int fd_ = -1;

public:
    Fd() = default;
    explicit Fd(int fd) : fd_(fd)
    {
        if (fd_ < 0)
            throw std::system_error(errno, std::generic_category(), "socket call failed");
    }
    Fd(const Fd &) = delete;
    Fd &operator=(const Fd &) = delete;
    Fd(Fd &&o) noexcept : fd_(std::exchange(o.fd_, -1)) {}
    Fd &operator=(Fd &&o) noexcept
    {
        if (this != &o)
        {
            reset();
            fd_ = std::exchange(o.fd_, -1);
        }
        return *this;
    }
    ~Fd() { reset(); }
    void reset() { if (fd_ >= 0) ::close(fd_); fd_ = -1; }
    int get() const { return fd_; }
};

std::string recv_some(const Fd &s)
{
    char buf[64];
    ssize_t n = ::recv(s.get(), buf, sizeof buf, 0);
    if (n < 0)
        throw std::system_error(errno, std::generic_category(), "recv");
    return std::string(buf, static_cast<std::size_t>(n));
}

int main()
{
    Fd server(::socket(AF_INET, SOCK_STREAM, 0));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;                                 // let the kernel choose a free port
    if (::bind(server.get(), reinterpret_cast<sockaddr *>(&addr), sizeof addr) < 0 || ::listen(server.get(), 1) < 0)
        throw std::system_error(errno, std::generic_category(), "bind/listen");
    socklen_t len = sizeof addr;
    ::getsockname(server.get(), reinterpret_cast<sockaddr *>(&addr), &len);

    std::string answer;
    std::thread client([&] {                           // the client runs in its own thread
        Fd c(::socket(AF_INET, SOCK_STREAM, 0));
        if (::connect(c.get(), reinterpret_cast<sockaddr *>(&addr), sizeof addr) < 0)
            return;
        const std::string msg = "hello from client";
        ::send(c.get(), msg.data(), msg.size(), 0);
        answer = recv_some(c);
    });

    Fd conn(::accept(server.get(), nullptr, nullptr));
    std::cout << "server got : " << recv_some(conn) << '\n';
    const std::string reply = "hello from server";
    ::send(conn.get(), reply.data(), reply.size(), 0);

    client.join();
    std::cout << "client got : " << answer << '\n';
}                                                      // all three descriptors are closed by their destructors
