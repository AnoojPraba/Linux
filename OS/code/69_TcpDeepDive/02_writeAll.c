#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// TCP is a BYTE STREAM, not a message stream: one write() may be split across
// many reads, many writes may be coalesced into one read, and write()/read()
// may transfer fewer bytes than requested. Correct code loops, and needs its
// own framing (length prefix or delimiter).
ssize_t write_all(int fd, const void *buf, size_t len)
{
    const char *p = buf;
    size_t left = len;
    while (left > 0)
    {
        ssize_t n = write(fd, p, left);
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;              // EAGAIN: caller must wait for writability
        }
        p += n;
        left -= n;
    }
    return len;
}

ssize_t read_exact(int fd, void *buf, size_t len)
{
    char *p = buf;
    size_t got = 0;
    while (got < len)
    {
        ssize_t n = read(fd, p + got, len - got);
        if (n == 0)
            return got;             // peer closed
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        got += n;
    }
    return got;
}

// Length-prefixed message framing: 4-byte big-endian length + payload.
static int send_msg(int fd, const char *s)
{
    unsigned len = strlen(s);
    unsigned char hdr[4] = { len >> 24, len >> 16, len >> 8, len };
    if (write_all(fd, hdr, 4) < 0) return -1;
    return write_all(fd, s, len) < 0 ? -1 : 0;
}

static int recv_msg(int fd, char *out, size_t cap)
{
    unsigned char hdr[4];
    if (read_exact(fd, hdr, 4) != 4) return -1;
    unsigned len = (unsigned)hdr[0] << 24 | hdr[1] << 16 | hdr[2] << 8 | hdr[3];
    if (len >= cap) return -1;                  // never trust a peer's length
    if (read_exact(fd, out, len) != (ssize_t)len) return -1;
    out[len] = '\0';
    return len;
}

int main(void)
{
    int sv[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, sv);    // stream semantics like TCP
    send_msg(sv[0], "first");
    send_msg(sv[0], "second message");
    send_msg(sv[0], "third");                   // three writes, maybe one read
    char buf[64];
    for (int i = 0; i < 3; i++)
        if (recv_msg(sv[1], buf, sizeof buf) >= 0)
            printf("message %d: \"%s\"\n", i + 1, buf);
    return 0;
}
