#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT                     8090
#define LINE_BUF_SIZE                   256
#define UNKNOWN_PROCEDURE_NAME_SIZE     64

/* -------------------------------------------------------------------------
 * This is the RPC-specific part of the demo: a tiny hand-rolled, line-based
 * text protocol standing in for a real RPC framework's wire format (e.g.
 * protobuf for gRPC). A request line is:
 *     PROCEDURE_NAME arg1 [arg2]
 * and the response is a single line back. Real RPC frameworks generate this
 * marshalling/dispatch code from a schema (see NOTES.md) - here it's all
 * written out by hand so every step (marshal, send, dispatch, unmarshal,
 * reply) is visible.
 * ------------------------------------------------------------------------- */

/*****************************************************************************
 * Name: reverseString
 *
 * Description:
 *         The "remote procedure" reverseString(s) - reverses a string
 *         in-place.
 *
 * Inputs:
 *         str : null-terminated string to reverse, modified in place.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void reverseString(char *str)
{
    int left = 0;
    int right = (int)strlen(str) - 1;

    while (left < right)
    {
        char tmp = str[left];

        str[left]  = str[right];
        str[right] = tmp;
        left++;
        right--;
    }
}

/*****************************************************************************
 * Name: dispatchRequest
 *
 * Description:
 *         Unmarshals a request line into a procedure name + arguments,
 *         dispatches to the matching "remote procedure", and marshals the
 *         result back into a response line. This is the request-dispatch
 *         table at the heart of any RPC server - a real framework generates
 *         this switch/lookup from an interface definition (.proto file,
 *         WSDL, etc.) instead of it being written by hand.
 *
 * Inputs:
 *         request  : incoming request line, e.g. "add 2 3" or "reverseString hello".
 *         response : buffer to write the response line into.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void dispatchRequest(char *request, char *response)
{
    char procedure[UNKNOWN_PROCEDURE_NAME_SIZE];
    int a, b;
    char arg[LINE_BUF_SIZE];

    if (sscanf(request, "add %d %d", &a, &b) == 2)
    {
        // Marshalling on the way out is just "format the result as text" -
        // a binary protocol would instead pack a fixed-width int.
        snprintf(response, LINE_BUF_SIZE, "%d", a + b);
    }
    else if (sscanf(request, "reverseString %s", arg) == 1)
    {
        reverseString(arg);
        snprintf(response, LINE_BUF_SIZE, "%s", arg);
    }
    else
    {
        sscanf(request, "%63s", procedure);
        snprintf(response, LINE_BUF_SIZE, "ERROR: unknown procedure '%s'", procedure);
    }
}

int main()
{
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    int clientFd;
    struct sockaddr_in address;
    int addrLen = sizeof(address);
    int reuseAddr = 1;
    char requestLine[LINE_BUF_SIZE];
    char responseLine[LINE_BUF_SIZE];
    ssize_t bytesReceived;

    // Socket setup below mirrors 49_SocketProgramming/01_tcpServer.c exactly
    // - see that folder for socket-API details. Everything RPC-specific is
    // in dispatchRequest() above.
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(SERVER_PORT);

    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind");
        return 1;
    }

    listen(serverFd, 1);

    printf("rpc server: listening on port %d\n", SERVER_PORT);

    clientFd = accept(serverFd, (struct sockaddr *)&address, (socklen_t *)&addrLen);

    bytesReceived              = read(clientFd, requestLine, LINE_BUF_SIZE - 1);
    requestLine[bytesReceived] = '\0';
    printf("rpc server: received request \"%s\"\n", requestLine);

    dispatchRequest(requestLine, responseLine);
    write(clientFd, responseLine, strlen(responseLine));

    close(clientFd);
    close(serverFd);
    return 0;
}
