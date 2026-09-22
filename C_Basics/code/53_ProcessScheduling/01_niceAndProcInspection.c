#include <stdio.h>
#include <unistd.h>

int main()
{
    // A process's "nice" value (-20 to 19) is a hint to the OS scheduler
    // about relative CPU priority - higher nice = more willing to yield
    // the CPU to others, lower (more negative) = more aggressive about
    // getting scheduled. It's a hint, not a guarantee, and only root can
    // lower it below the default.
    int currentNice = nice(0);
    char line[256];
    FILE *fp;

    printf("current nice value = %d\n", currentNice);

    // nice(5) both raises this process's niceness by 5 and returns the new
    // value - a lower-priority process yields to others more readily.
    printf("nice(5) -> new value = %d\n", nice(5));

    // /proc/self is a live view of this exact running process's kernel
    // state - /proc/self/status has already been used to read RSS in
    // 57_Paging/02_demandPaging.c; here it's read for the scheduling-
    // relevant fields instead.
    fp = fopen("/proc/self/status", "r");
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if ((line[0] == 'S') && (line[1] == 't') && (line[2] == 'a') &&
            (line[3] == 't') && (line[4] == 'e'))
        {
            printf("%s", line);
        }
    }
    fclose(fp);

    return 0;
}
