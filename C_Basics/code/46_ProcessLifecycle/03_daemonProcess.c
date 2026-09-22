#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOG_PATH "/tmp/c_basics_daemon_demo.log"

// The classic double-fork daemonization sequence: it detaches a process
// entirely from the terminal that started it, so it keeps running as a
// background service instead of dying when that terminal closes.
int main()
{
    pid_t pid = fork();
    FILE *logFile;

    if (pid < 0)
    {
        return 1;
    }
    if (pid > 0)
    {
        // The original process exits immediately - its child (below)
        // continues on, already orphaned and re-parented to init, exactly
        // like 02_orphanProcess.c demonstrates directly.
        return 0;
    }

    // setsid() creates a new session with this process as its leader,
    // detaching it from the controlling terminal - without this, signals
    // sent to the terminal (e.g. Ctrl+C) could still reach it.
    setsid();

    // A second fork (this process exiting, its child continuing) is the
    // traditional extra step that guarantees the final daemon process is
    // not itself a session leader, so it can never accidentally reacquire
    // a controlling terminal.
    pid = fork();
    if (pid < 0)
    {
        return 1;
    }
    if (pid > 0)
    {
        return 0;
    }

    // A daemon has no terminal to print to - anything it wants to record
    // has to go to a log file (or syslog) instead of stdout.
    logFile = fopen(LOG_PATH, "a");
    if (logFile != NULL)
    {
        fprintf(logFile, "daemon (pid %d) started\n", getpid());
        fclose(logFile);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    return 0;
}
