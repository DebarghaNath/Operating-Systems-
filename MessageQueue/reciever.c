#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
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
    long int MsgType = 1; 
    int MsgId = msgget(MsgKey, IPC_CREAT | 0666);
    if (MsgId == -1)
    {
        perror("MSGGET");
        exit(1);
    }
    
    Msg NewMsg;
    int running = 1;
    
    while (running)
    {
        if (msgrcv(MsgId, (void *)&NewMsg, MaxText, MsgType, 0) == -1)
        {
            perror("MSGRCV");
            exit(1);
        }
        NewMsg.Msg[strcspn(NewMsg.Msg, "\n")] = '\0';

        printf("RECEIVED: %s\n", NewMsg.Msg);
        
        if (strncmp(NewMsg.Msg, "end", 3) == 0)
        {
            running = 0;
        }
    }

    if (msgctl(MsgId, IPC_RMID, 0) == -1)
    {
        perror("MSGCTL");
        exit(1);
    }
    return 0;
}
