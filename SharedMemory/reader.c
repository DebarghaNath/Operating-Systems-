#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    key_t key = ftok("shmfile.txt",0);
    int block = shmget(key,1024,0666|IPC_CREAT);
    char * str = shmat(block,NULL,0);

    printf("VALUE READ FROM SHM: ");
    printf("%s",str);
    shmdt(str);
 
    // destroy the shared memory
    shmctl(block, IPC_RMID, NULL);

}