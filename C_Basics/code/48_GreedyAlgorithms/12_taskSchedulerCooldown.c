// Task scheduler with a cooldown: the minimum time to run tasks when equal
// tasks must be n slots apart (counting formula and heap simulation).

#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *tasks = "AAABBBCCD";
    int n = 2;                                           /* cooldown between two equal tasks */
    int freq[26] = {0}, len = (int)strlen(tasks), max = 0, num_max = 0;
    for (int i = 0; i < len; i++) freq[tasks[i] - 'A']++;
    for (int i = 0; i < 26; i++)
    {
        if (freq[i] > max) { max = freq[i]; num_max = 1; }
        else if (freq[i] == max) num_max++;
    }
    /* (max-1) full frames of n+1 slots, then the last row holds every task that reaches the maximum count */
    int frames = (max - 1) * (n + 1) + num_max;
    int answer = frames > len ? frames : len;            /* never less than the task count (no idle needed) */
    printf("tasks %s with cooldown %d need %d time units\n", tasks, n, answer);

    /* simulation: each unit run the available task with the most remaining work */
    int remaining[26], next_ok[26] = {0}, done = 0, t = 0;
    memcpy(remaining, freq, sizeof freq);
    while (done < len)
    {
        int pick = -1;
        for (int i = 0; i < 26; i++)
            if (remaining[i] > 0 && next_ok[i] <= t && (pick < 0 || remaining[i] > remaining[pick])) pick = i;
        if (pick >= 0)
        {
            remaining[pick]--;
            next_ok[pick] = t + n + 1;
            done++;
        }
        t++;                                              /* a unit passes whether we ran a task or idled */
    }
    printf("greedy simulation finishes at time %d\n", t);
    return 0;
}
