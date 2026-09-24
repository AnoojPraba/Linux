#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT   8090
#define LINE_BUF_SIZE 256

/*****************************************************************************
 * Name: callRemoteProcedure
 *
 * Description:
 *         Makes the network call "look like" a local function call from the
 *         caller's point of view: marshals procedureCall into a request
 *         line, sends it, blocks for the response, and returns it. This is
 *         the client-side "stub" - a real RPC framework generates this
 *         function automatically from a schema instead of it being hand-
 *         written per call.
 *
 * Inputs:
 *         sockFd        : connected socket to the RPC server.
 *         procedureCall : request line, e.g. "add 2 3".
 *         response      : buffer to receive the response line into.
 *         responseSize  : size of the response buffer.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void callRemoteProcedure(int sockFd, const char *procedureCall, char *response,
                          size_t responseSize)
{
    ssize_t bytesReceived;

    write(sockFd, procedureCall, strlen(procedureCall));
    bytesReceived = read(sockFd, response, responseSize - 1);
    response[bytesReceived > 0 ? bytesReceived : 0] = '\0';
}

int main()
{
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    char response[LINE_BUF_SIZE];

    // Socket setup mirrors 49_SocketProgramming/02_tcpClient.c - see that
    // folder for socket-API details. One connection, one request/response
    // pair, then disconnect - keeps this demo focused on the RPC framing.
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port   = htons(SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    if (connect(sockFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("rpc client: connect failed - is the rpc server running?\n");
        return 1;
    }

    // From here, calling a "remote procedure" reads like calling a local
    // function - the marshalling/network round trip is hidden inside
    // callRemoteProcedure(). This is the entire point of RPC.
    callRemoteProcedure(sockFd, "add 2 3", response, sizeof(response));
    printf("rpc client: add(2, 3) = %s\n", response);

    close(sockFd);
    return 0;
}
