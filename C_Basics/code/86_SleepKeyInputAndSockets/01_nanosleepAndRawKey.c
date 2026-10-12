// nanosleep restarted after EINTR, elapsed-time check with CLOCK_MONOTONIC,
// and raw single-key input via termios (conio getch equivalent).

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

/* Sleep for the given number of milliseconds, restarting after a signal. */
static void sleep_ms(long ms)
{
    struct timespec req = { ms / 1000, (ms % 1000) * 1000000L }, rem;
    while (nanosleep(&req, &rem) == -1)
        req = rem;                              /* interrupted: sleep for the remaining time */
}

/* Read one key press without echo and without waiting for Enter (like getch on Windows). */
static int get_key(void)
{
    struct termios old, raw;
    if (tcgetattr(STDIN_FILENO, &old) == -1)
        return getchar();                       /* not a terminal: fall back to normal reading */
    raw = old;
    raw.c_lflag &= ~(tcflag_t)(ICANON | ECHO);  /* byte-at-a-time input, no echo */
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    int c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old);     /* always restore the terminal */
    return c;
}

int main(void)
{
    struct timespec a, b;
    clock_gettime(CLOCK_MONOTONIC, &a);
    sleep_ms(120);
    clock_gettime(CLOCK_MONOTONIC, &b);
    long elapsed = (b.tv_sec - a.tv_sec) * 1000 + (b.tv_nsec - a.tv_nsec) / 1000000;
    printf("slept at least 120 ms: %s\n", elapsed >= 120 ? "yes" : "no");

    int key = isatty(STDIN_FILENO) ? get_key() : 'x';   /* keep this demo non-interactive */
    printf("key code: %d\n", key);
    return 0;
}
