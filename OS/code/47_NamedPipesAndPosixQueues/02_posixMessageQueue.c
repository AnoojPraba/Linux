#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/wait.h>

#define MQ_NAME     "/85_demo_mq"
#define MQ_MAX_MSGS 10
#define MQ_MSG_SIZE 128

int main()
{
    struct mq_attr attr;
    mqd_t mq;
    pid_t pid;

    attr.mq_flags   = 0;
    attr.mq_maxmsg  = MQ_MAX_MSGS;
    attr.mq_msgsize = MQ_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // POSIX message queues are named like files (leading '/', no further
    // slashes) rather than keyed by an integer like the SysV queue in
    // 46_SystemVIPC/02_messageQueue.c. mq_send()'s priority argument (0
    // here) lets higher-priority messages jump ahead of lower-priority ones
    // already queued - SysV msgsnd()/msgrcv() instead uses a message "type"
    // for selective receive, not a true priority ordering.
    mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0666, &attr);

    pid = fork();

    if (pid == 0)
    {
        const char *message = "hello via posix message queue";

        mq_send(mq, message, strlen(message) + 1, 0);
        mq_close(mq);
        _exit(0);
    }
    else
    {
        char buffer[MQ_MSG_SIZE];
        unsigned int priority;
        ssize_t bytesRead;

        waitpid(pid, NULL, 0);
        bytesRead = mq_receive(mq, buffer, MQ_MSG_SIZE, &priority);
        buffer[bytesRead > 0 ? bytesRead : 0] = '\0';
        printf("parent received: %s\n", buffer);

        mq_close(mq);
        mq_unlink(MQ_NAME);
    }

    return 0;
}
