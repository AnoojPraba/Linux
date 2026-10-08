#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

// One-to-many UDP. Broadcast (255.255.255.255 / subnet) reaches every host on
// the LAN segment (needs SO_BROADCAST, not routed). Multicast (224.0.0.0/4)
// reaches only receivers that JOIN the group (IP_ADD_MEMBERSHIP) and can cross
// routers with IGMP/PIM. Here: multicast sent and received on the loopback
// interface so it works on one machine; if the host has no multicast route for
// lo the program reports it and exits cleanly.
#define GROUP "239.255.42.99"

int main(void)
{
    int rx = socket(AF_INET, SOCK_DGRAM, 0);
    int one = 1;
    setsockopt(rx, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);   // several receivers per port
    struct sockaddr_in a = { .sin_family = AF_INET, .sin_addr.s_addr = htonl(INADDR_ANY) };
    bind(rx, (struct sockaddr *)&a, sizeof a);
    socklen_t al = sizeof a;
    getsockname(rx, (struct sockaddr *)&a, &al);
    int port = ntohs(a.sin_port);

    struct ip_mreqn mreq;
    memset(&mreq, 0, sizeof mreq);
    inet_pton(AF_INET, GROUP, &mreq.imr_multiaddr);
    mreq.imr_address.s_addr = htonl(INADDR_LOOPBACK);
    mreq.imr_ifindex = 0;
    if (setsockopt(rx, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof mreq) < 0)
    {
        printf("join failed (%s) - skipping multicast demo\n", strerror(errno));
        return 0;
    }

    int tx = socket(AF_INET, SOCK_DGRAM, 0);
    struct in_addr out_if = { .s_addr = htonl(INADDR_LOOPBACK) };
    setsockopt(tx, IPPROTO_IP, IP_MULTICAST_IF, &out_if, sizeof out_if);   // pick interface
    setsockopt(tx, IPPROTO_IP, IP_MULTICAST_LOOP, &one, sizeof one);       // deliver to local receivers
    unsigned char ttl = 1;                                                  // stay on this segment
    setsockopt(tx, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof ttl);

    struct sockaddr_in dst = { .sin_family = AF_INET, .sin_port = htons(port) };
    inet_pton(AF_INET, GROUP, &dst.sin_addr);
    if (sendto(tx, "hello group", 11, 0, (struct sockaddr *)&dst, sizeof dst) < 0)
    {
        printf("multicast send failed (%s) - skipping\n", strerror(errno));
        return 0;
    }

    struct timeval tv = { .tv_sec = 2 };
    setsockopt(rx, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
    char buf[64];
    ssize_t n = recv(rx, buf, sizeof buf - 1, 0);
    if (n > 0)
    {
        buf[n] = '\0';
        printf("receiver in group %s got: \"%s\"\n", GROUP, buf);
    }
    else
        printf("no datagram received (%s)\n", strerror(errno));

    // Broadcast needs SO_BROADCAST; shown for the API, sent to loopback-scoped
    // limited broadcast only if the stack allows it.
    int bc = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(bc, SOL_SOCKET, SO_BROADCAST, &one, sizeof one);
    printf("SO_BROADCAST set; send to 255.255.255.255:%d would reach the whole LAN segment\n", port);
    close(bc); close(tx); close(rx);
    return 0;
}
