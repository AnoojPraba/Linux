#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main()
{
    struct winsize windowSize;

    // ioctl() is the catch-all syscall for device-specific operations that
    // don't fit read/write/open/close - here, asking the terminal driver
    // for its current window size. Different device types (terminals,
    // sound cards, network interfaces) each define their own set of
    // ioctl request codes, unlike the uniform read/write interface most
    // devices also support.
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) == 0)
    {
        printf("terminal size: %d rows x %d columns\n",
               windowSize.ws_row, windowSize.ws_col);
    }
    else
    {
        // Fails with ENOTTY if stdout isn't an actual terminal (e.g. it
        // was redirected to a file or a pipe) - there's no window size to
        // report in that case.
        printf("stdout is not a terminal, ioctl(TIOCGWINSZ) not applicable\n");
    }

    return 0;
}
