#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main()
{
    //HERE I HAVE CREATED A UNIQUE KEY FOR FILE THAT I WANT IN SHARED MEMORY
    key_t key  = ftok("shmfile.txt",0);
    
    //HERE T GENERATE A SHMID CALLED BLOCK FOR THAT KEY AND ASSIGNING 1024 BYTES TO IT (1KB)
    //GIVING READ,WRITE IF FILE EXISTS OR IF NOT CREATE AND ASSIGN THAT PERMISSIONS
    int block = shmget(key,1024,0666|IPC_CREAT);
    //LINK THAT BLOCK OF SHM TO OUR PROCESS CURRENTLY RUNNING
    char * str = (char *)shmat(block,NULL,0);

    //HERE WE WRITE INTO SHARED MEMORY WHICH IS A FILE HERE
    printf("ENTER VALUE: ");
    scanf("%s", str);
    printf("Value written to shared memory: %s\n", str);

    // DEATACH THE SHARED MEMORY
    shmdt(str);
    return 0;
}