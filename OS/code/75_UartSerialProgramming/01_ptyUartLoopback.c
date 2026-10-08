#define _XOPEN_SOURCE 600
#define _DEFAULT_SOURCE
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

// UART programming on Linux uses the TERMINAL (tty) layer: a serial port is a
// character device (/dev/ttyAMA0, /dev/ttyUSB0, /dev/serial0) configured with
// termios. No hardware here, so we use a pseudo-terminal pair: the "slave" end
// behaves exactly like a serial port to the application, while a thread plays
// the role of the remote device on the "master" end (here: uppercases bytes).
//
// Real serial port = same code; just open("/dev/ttyUSB0") instead of the pty.
// (A pty has no baud rate/parity/electrical layer - those settings are accepted
// but not enforced.)
static int master_fd;

static void *remote_device(void *arg)
{
    (void)arg;
    char c;
    while (read(master_fd, &c, 1) == 1)
    {
        if (c == '\n' || c == '\r')                 // device replies per line
        {
            write(master_fd, "\r\n", 2);
            continue;
        }
        c = toupper((unsigned char)c);
        write(master_fd, &c, 1);
    }
    return NULL;
}

static void configure_8n1(int fd, speed_t baud)
{
    struct termios t;
    tcgetattr(fd, &t);
    cfmakeraw(&t);                  // raw: no echo, no line editing, no CR/LF mangling,
                                    // no signal chars (^C) - what binary protocols need
    cfsetispeed(&t, baud);
    cfsetospeed(&t, baud);
    t.c_cflag &= ~(PARENB | CSTOPB | CSIZE);   // no parity, 1 stop bit
    t.c_cflag |= CS8 | CLOCAL | CREAD;         // 8 data bits, ignore modem lines, enable receiver
    t.c_cflag &= ~CRTSCTS;                     // no hardware flow control
    t.c_iflag &= ~(IXON | IXOFF | IXANY);      // no software (XON/XOFF) flow control
    t.c_cc[VMIN] = 0;               // read() returns whatever is available...
    t.c_cc[VTIME] = 5;              // ...or after 0.5 s (units: 1/10 s) with nothing
    tcsetattr(fd, TCSANOW, &t);
    tcflush(fd, TCIOFLUSH);         // discard stale bytes
}

int main(void)
{
    master_fd = posix_openpt(O_RDWR | O_NOCTTY);
    if (master_fd < 0 || grantpt(master_fd) < 0 || unlockpt(master_fd) < 0)
    {
        perror("pty");
        return 1;
    }
    const char *slave_name = ptsname(master_fd);
    printf("pseudo serial port: %s\n", slave_name);

    // Put the master in raw mode too, or its line discipline would echo/edit.
    struct termios mt;
    tcgetattr(master_fd, &mt);
    cfmakeraw(&mt);
    tcsetattr(master_fd, TCSANOW, &mt);

    pthread_t th;
    pthread_create(&th, NULL, remote_device, NULL);

    int fd = open(slave_name, O_RDWR | O_NOCTTY);
    configure_8n1(fd, B115200);

    struct termios t;
    tcgetattr(fd, &t);
    printf("configured: %u baud, %s, parity %s, stop bits %d, flow control %s\n",
           115200u, (t.c_cflag & CSIZE) == CS8 ? "8 data bits" : "other",
           (t.c_cflag & PARENB) ? "on" : "none", (t.c_cflag & CSTOPB) ? 2 : 1,
           (t.c_cflag & CRTSCTS) ? "RTS/CTS" : "none");
    printf("VMIN=%d VTIME=%d (read returns after 0.5s timeout if nothing)\n",
           t.c_cc[VMIN], t.c_cc[VTIME]);

    const char *msg = "at+hello\n";
    write(fd, msg, strlen(msg));
    tcdrain(fd);                    // wait until all bytes have been TRANSMITTED

    char buf[64];
    size_t got = 0;
    while (got < sizeof buf - 1)
    {
        ssize_t n = read(fd, buf + got, sizeof buf - 1 - got);
        if (n <= 0)
            break;                  // timeout (VTIME) or error
        got += n;
        if (memchr(buf, '\n', got))
            break;                  // got a full reply line
    }
    buf[got] = '\0';
    for (char *p = buf; *p; p++)
        if (*p == '\r' || *p == '\n')
            *p = '$';
    printf("sent \"at+hello\\n\", device replied (CR/LF shown as $): \"%s\"\n", buf);

    // Timeout behavior: nothing more is coming, VMIN=0/VTIME=5 returns 0.
    ssize_t n = read(fd, buf, sizeof buf);
    printf("idle read -> %zd bytes (timeout, not an error)\n", n);

    close(fd);
    close(master_fd);
    pthread_join(th, NULL);
    return 0;
}
