#include <stdio.h>
#include <string.h>

// cgroups (v2) = resource LIMITS and accounting for a group of processes, as
// files under /sys/fs/cgroup. Namespaces limit what a process can SEE;
// cgroups limit what it can USE. This program just reads our own cgroup.
static void show(const char *label, const char *path)
{
    char buf[256] = "(unreadable)";
    FILE *f = fopen(path, "r");
    if (f)
    {
        if (!fgets(buf, sizeof buf, f))
            strcpy(buf, "(empty)");
        fclose(f);
        buf[strcspn(buf, "\n")] = '\0';
    }
    printf("%-22s %s\n", label, buf);
}

int main(void)
{
    char line[256], cg[200] = "/";
    FILE *f = fopen("/proc/self/cgroup", "r");     // v2 line: "0::/path"
    if (f)
    {
        if (fgets(line, sizeof line, f))
        {
            char *p = strstr(line, "::");
            if (p)
            {
                snprintf(cg, sizeof cg, "%s", p + 2);
                cg[strcspn(cg, "\n")] = '\0';
            }
        }
        fclose(f);
    }
    printf("my cgroup: %s\n", cg);

    char path[512];
    const char *files[] = { "cgroup.controllers", "memory.max", "memory.current",
                            "cpu.max", "pids.max", "pids.current" };
    for (size_t i = 0; i < sizeof files / sizeof *files; i++)
    {
        snprintf(path, sizeof path, "/sys/fs/cgroup%s/%s", cg, files[i]);
        show(files[i], path);
    }
    printf("\nTo limit a process (needs a delegated/own cgroup, usually root):\n"
           "  mkdir /sys/fs/cgroup/demo\n"
           "  echo 100M > /sys/fs/cgroup/demo/memory.max   # OOM-kill above this\n"
           "  echo '50000 100000' > /sys/fs/cgroup/demo/cpu.max   # 50%% of one CPU\n"
           "  echo $$ > /sys/fs/cgroup/demo/cgroup.procs\n");
    return 0;
}
