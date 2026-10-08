#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <unistd.h>

// A "container" is just a process with new namespaces (+ cgroups + a root fs).
// Here: user, PID, UTS (hostname), mount namespaces via clone(2), no root needed
// (unprivileged user namespaces; may be disabled on some distros).
//   CLONE_NEWUSER  uid/gid mapping: be "root" inside, unprivileged outside
//   CLONE_NEWPID   child sees itself as PID 1
//   CLONE_NEWUTS   private hostname
//   CLONE_NEWNS    private mount table (so a fresh /proc doesn't leak out)
#define STACK_SIZE (1024 * 1024)

static int pipefd[2];

static int child(void *arg)
{
    (void)arg;
    char c;
    close(pipefd[1]);
    read(pipefd[0], &c, 1);             // wait until the parent wrote uid_map

    sethostname("container", 9);
    printf("child:  pid=%d uid=%d (inside the namespaces)\n", getpid(), getuid());

    // Make mounts private, then mount a PID-namespace-aware /proc.
    if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) < 0)
        perror("mount private");
    if (mount("proc", "/proc", "proc", 0, NULL) < 0)
        perror("mount /proc");

    char host[64];
    gethostname(host, sizeof host);
    printf("child:  hostname=%s\n", host);
    printf("child:  processes visible in /proc:\n");
    fflush(stdout);
    execlp("sh", "sh", "-c", "ls /proc | grep -E '^[0-9]+$' | tr '\\n' ' '; echo", (char *)NULL);
    perror("exec");
    return 1;
}

static void write_file(const char *path, const char *s)
{
    int fd = open(path, O_WRONLY);
    if (fd < 0 || write(fd, s, strlen(s)) < 0)
        perror(path);
    if (fd >= 0)
        close(fd);
}

int main(void)
{
    pipe(pipefd);
    char *stack = malloc(STACK_SIZE);
    uid_t uid = getuid();
    gid_t gid = getgid();

    pid_t pid = clone(child, stack + STACK_SIZE,
                      CLONE_NEWUSER | CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS | SIGCHLD,
                      NULL);
    if (pid < 0)
    {
        perror("clone (are unprivileged user namespaces enabled?)");
        return 1;
    }

    // Map uid 0 inside -> our real uid outside (must be done by the parent).
    char path[64], map[64];
    snprintf(path, sizeof path, "/proc/%d/setgroups", pid);
    write_file(path, "deny");
    snprintf(path, sizeof path, "/proc/%d/uid_map", pid);
    snprintf(map, sizeof map, "0 %d 1", uid);
    write_file(path, map);
    snprintf(path, sizeof path, "/proc/%d/gid_map", pid);
    snprintf(map, sizeof map, "0 %d 1", gid);
    write_file(path, map);

    printf("parent: child is pid %d on the host, my uid=%d\n", pid, uid);
    close(pipefd[0]);
    write(pipefd[1], "x", 1);           // release the child
    waitpid(pid, NULL, 0);
    return 0;
}
