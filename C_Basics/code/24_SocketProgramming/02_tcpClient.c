#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define RECV_BUF_SIZE 128

int main()
{
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    const char *message = "hello from client";
    char buffer[RECV_BUF_SIZE];
    ssize_t bytesReceived;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    // inet_pton converts a human-readable IP string into the packed binary
    // form the socket API needs - loopback here means client and server
    // must be running on the same machine.
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    if (connect(sockFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("client: connect failed - is the server running?\n");
        return 1;
    }

    write(sockFd, message, strlen(message));

    bytesReceived = read(sockFd, buffer, RECV_BUF_SIZE - 1);
    buffer[bytesReceived] = '\0';
    printf("client: received \"%s\"\n", buffer);

    close(sockFd);
    return 0;
}
