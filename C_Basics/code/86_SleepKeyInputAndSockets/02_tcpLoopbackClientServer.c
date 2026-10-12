// socket/bind/listen/accept server and connect/send/recv client in one program
// (fork), port chosen by the kernel.

#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    /* ---- server: socket, bind, listen ---- */
    int srv = socket(AF_INET, SOCK_STREAM, 0);
    if (srv < 0) { perror("socket"); return 1; }
    int yes = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;                                  /* 0 = let the kernel pick a free port */
    if (bind(srv, (struct sockaddr *)&addr, sizeof addr) < 0) { perror("bind"); return 1; }
    socklen_t len = sizeof addr;
    getsockname(srv, (struct sockaddr *)&addr, &len);   /* learn which port we got */
    if (listen(srv, 1) < 0) { perror("listen"); return 1; }

    pid_t pid = fork();
    if (pid == 0)
    {
        /* ---- client: socket, connect, send, receive ---- */
        close(srv);
        int c = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(c, (struct sockaddr *)&addr, sizeof addr) < 0) { perror("connect"); _exit(1); }
        const char *msg = "hello from client";
        send(c, msg, strlen(msg), 0);
        char reply[64];
        ssize_t n = recv(c, reply, sizeof reply - 1, 0);
        if (n < 0) n = 0;
        reply[n] = '\0';
        close(c);
        _exit(strcmp(reply, "hello from server") == 0 ? 0 : 1);   /* report through the exit status */
    }

    /* ---- server: accept, receive, reply ---- */
    int conn = accept(srv, NULL, NULL);
    char buf[64];
    ssize_t n = recv(conn, buf, sizeof buf - 1, 0);
    if (n < 0) n = 0;
    buf[n] = '\0';
    printf("server got : %s\n", buf);
    const char *ans = "hello from server";
    send(conn, ans, strlen(ans), 0);
    close(conn);
    close(srv);
    int status = 0;
    waitpid(pid, &status, 0);                           /* collect the client process */
    printf("client got the reply: %s\n", WIFEXITED(status) && WEXITSTATUS(status) == 0 ? "yes" : "no");
    return 0;
}
