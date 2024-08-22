#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MsgKey 100
#define MaxText 50

typedef struct 
{
    long int Msg_type;
    char Msg[MaxText];
} Msg;

int main()
{
    char buffer[20];
    int MsgId = msgget(MsgKey, IPC_CREAT | 0666);
    if (MsgId == -1)
    {
        perror("MSGGET");
        exit(1);
    }

    int running = 1;
    Msg NewMsg;

    while (running)
    {
        printf("ENTER SOME TEXT \n");
        fgets(buffer, 20, stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        NewMsg.Msg_type = 1;
        strcpy(NewMsg.Msg, buffer);
        if (msgsnd(MsgId, (void *)&NewMsg, strlen(NewMsg.Msg) + 1, 0) == -1)
        {
            perror("Message not sent");
        }
        if (strncmp(buffer, "end", 3) == 0)
        {
            running = 0;
        }
    }
    return 0;
}
