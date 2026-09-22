#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8081
#define RECV_BUF_SIZE 128

int main()
{
    // SOCK_DGRAM (vs. TCP's SOCK_STREAM in 69_SocketProgramming/) means no
    // connection is ever established - there's no accept(), and no
    // guarantee a sent datagram arrives, arrives once, or arrives in
    // order. UDP trades TCP's reliability for lower overhead and no
    // connection setup latency - appropriate when occasional loss is
    // acceptable (e.g. live video/audio, some game state updates) or when
    // the application layer handles its own retries.
    int sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[RECV_BUF_SIZE];
    ssize_t bytesReceived;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    bind(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    printf("UDP server: listening on port %d\n", SERVER_PORT);

    // recvfrom (not read/accept) both receives the datagram and fills in
    // who it came from - each call can be from a different sender, since
    // there's no persistent per-client connection like TCP's accept()
    // returns.
    bytesReceived = recvfrom(sockFd, buffer, RECV_BUF_SIZE - 1, 0,
                              (struct sockaddr *)&clientAddr, &clientAddrLen);
    buffer[bytesReceived] = '\0';
    printf("server: received \"%s\"\n", buffer);

    sendto(sockFd, "hello from UDP server", 22, 0,
           (struct sockaddr *)&clientAddr, clientAddrLen);

    close(sockFd);
    return 0;
}
