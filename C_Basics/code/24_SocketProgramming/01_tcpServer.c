#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define RECV_BUF_SIZE 128

int main()
{
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    int clientFd;
    struct sockaddr_in address;
    int addrLen = sizeof(address);
    char buffer[RECV_BUF_SIZE];
    int reuseAddr = 1;
    ssize_t bytesReceived;
    const char *reply = "hello from server";

    // SO_REUSEADDR lets the socket rebind to a port still lingering in
    // TIME_WAIT from this same program's previous run, instead of failing
    // with "address already in use".
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    bind(serverFd, (struct sockaddr *)&address, sizeof(address));
    // backlog of 1: only one pending connection is queued before accept()
    listen(serverFd, 1);

    printf("server: listening on port %d\n", SERVER_PORT);

    // accept() blocks until a client connects, then returns a brand new
    // file descriptor for that specific connection - serverFd stays free to
    // accept further connections.
    clientFd = accept(serverFd, (struct sockaddr *)&address, (socklen_t *)&addrLen);

    bytesReceived = read(clientFd, buffer, RECV_BUF_SIZE - 1);
    buffer[bytesReceived] = '\0';
    printf("server: received \"%s\"\n", buffer);

    write(clientFd, reply, strlen(reply));

    close(clientFd);
    close(serverFd);
    return 0;
}
