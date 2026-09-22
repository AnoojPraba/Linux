#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8081
#define RECV_BUF_SIZE 128

int main()
{
    int sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serverAddr;
    char buffer[RECV_BUF_SIZE];
    ssize_t bytesReceived;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // No connect() call needed before sendto() (unlike TCP's connect() in
    // 69_SocketProgramming/02_tcpClient.c) - every sendto() specifies the
    // destination address itself, since UDP has no persistent connection
    // to remember it.
    sendto(sockFd, "hello from UDP client", 22, 0,
           (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    bytesReceived = recvfrom(sockFd, buffer, RECV_BUF_SIZE - 1, 0, NULL, NULL);
    buffer[bytesReceived] = '\0';
    printf("client: received \"%s\"\n", buffer);

    close(sockFd);
    return 0;
}
