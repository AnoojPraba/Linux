#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

#define MSG_KEY 0x5678
#define MSG_TEXT_SIZE 64

typedef struct
{
    long msgType;
    char text[MSG_TEXT_SIZE];
} Message;

int main()
{
    // A message queue is a kernel-managed list of discrete messages, each
    // tagged with a caller-chosen msgType - unlike a pipe's undifferentiated
    // byte stream (45_IPC/01_pipeCommunication.c), a receiver can selectively
    // read only messages of a given type, out of arrival order if it wants.
    int msqId = msgget(MSG_KEY, IPC_CREAT | 0666);
    pid_t pid = fork();

    if (pid == 0)
    {
        Message msg;

        msg.msgType = 1;
        strcpy(msg.text, "hello from child via message queue");
        msgsnd(msqId, &msg, sizeof(msg.text), 0);
        return 0;
    }
    else
    {
        Message received;

        waitpid(pid, NULL, 0);
        // The 0 for msgType here means "receive the first message in the
        // queue regardless of type" - passing a specific positive type
        // would only receive messages tagged with exactly that type.
        msgrcv(msqId, &received, sizeof(received.text), 0, 0);
        printf("parent received: %s\n", received.text);

        msgctl(msqId, IPC_RMID, NULL);
    }

    return 0;
}
