#define _DEFAULT_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

// Generic serial-port terminal for a REAL device (GPS, modem, microcontroller
// debug UART): open, configure 8N1 raw, send an optional string, then print
// whatever arrives for a few seconds using poll() with a timeout.
//   ./02_openRealSerialPort [device] [baud] [text-to-send]
//   e.g. ./02_openRealSerialPort /dev/ttyUSB0 9600 "AT\r\n"
// Exits cleanly with an explanation if the port is missing or not permitted
// (user must be in the `dialout` group on most distros).
static speed_t baud_const(int b)
{
    switch (b)
    {
    case 9600:   return B9600;
    case 19200:  return B19200;
    case 38400:  return B38400;
    case 57600:  return B57600;
    case 115200: return B115200;
    case 230400: return B230400;
    case 921600: return B921600;
    default:     return 0;
    }
}

int main(int argc, char **argv)
{
    const char *dev = argc > 1 ? argv[1] : "/dev/serial0";
    int baud = argc > 2 ? atoi(argv[2]) : 115200;
    const char *text = argc > 3 ? argv[3] : NULL;
    speed_t sp = baud_const(baud);
    if (!sp)
    {
        fprintf(stderr, "unsupported baud %d\n", baud);
        return 1;
    }

    // O_NOCTTY: don't let the port become our controlling terminal (^C/hangup
    // semantics). O_NONBLOCK: don't hang in open() waiting for carrier detect.
    int fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0)
    {
        printf("cannot open %s: %s\n(no serial hardware, or not in the 'dialout' group)\n",
               dev, strerror(errno));
        return 0;
    }
    struct termios t;
    if (tcgetattr(fd, &t) < 0)
    {
        printf("%s is not a terminal device: %s\n", dev, strerror(errno));
        return 0;
    }
    cfmakeraw(&t);
    cfsetspeed(&t, sp);
    t.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS | CSIZE);
    t.c_cflag |= CS8 | CLOCAL | CREAD;
    tcsetattr(fd, TCSANOW, &t);
    tcflush(fd, TCIOFLUSH);
    printf("%s opened at %d 8N1\n", dev, baud);

    if (text)
    {
        write(fd, text, strlen(text));
        tcdrain(fd);
    }
    struct pollfd p = { .fd = fd, .events = POLLIN };
    int idle_ms = 0;
    while (idle_ms < 2000)
    {
        int r = poll(&p, 1, 200);
        if (r == 0) { idle_ms += 200; continue; }
        if (r < 0 && errno != EINTR) break;
        char buf[128];
        ssize_t n = read(fd, buf, sizeof buf);
        if (n > 0)
        {
            idle_ms = 0;
            fwrite(buf, 1, n, stdout);
            fflush(stdout);
        }
        else if (n == 0 || (n < 0 && errno != EAGAIN && errno != EINTR))
            break;
    }
    printf("\n(done)\n");
    close(fd);
    return 0;
}
