/*
    AUTHOR: DEBARGHA NATH
    DATE: 21/08/2024
*/

//LIBRARIES: 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <fcntl.h>

/*FOR ACCESSING THE SHARED MEMORY BLOCK:*/

//DEFINE STARTING LOCATION OF THE SHARED MEMORY SEGMENT 
#define SHM_KEY 10
//DEFINE FIXED SIZE OF BUFFER AS 100 WHICH WHILL ALSO BE THE FILE SIZE
#define BUFFER_SIZE 100

//IN SHM WE WOULD STORE THE POINTERS FOR CONSUMER AND PRODUCER IN A CIRCULAR BUFFER
typedef struct 
{
    int p;
    int c;
}SHM_NEW;

//CHILD PROCESS PROTOTYPE
void producer();
void consumer();

int main()
{
    // HERE WE DEFINE THE BLOCK AS SHARED MEMEORY 
    // STARTING INDEX AS SHM_KEY,SIZEOF :SHM_NEW
    //IPC_CREAT: CREATES THE SEGMENT IF IT DOES NOT EXISTS
    //0666: SETS PERMISSION OF THE SEGMENT AS READABLE AND WRITABLE BY EVERYONE
    int shmid = shmget(SHM_KEY,sizeof(SHM_NEW),IPC_CREAT|0666);
    if(shmid==-1)
    {
        perror("SHMGET");
        exit(1);
    }
    //shmat() GIVES A POINTER TO THE STARTING LOCATION WHERE THE SEGMENT IS ADDED INTO CURRENT PROCESS'S ADDRESS
    //shmid: UNIQUE KEY FOR SEGMENT
    //NULL: NO SPECIFIC LOCATION TO DO THE ADDITION
    SHM_NEW *shm_ptr;
    shm_ptr = (SHM_NEW*)shmat(shmid,NULL,0);
    if(shm_ptr== (SHM_NEW*)-1)
    {
        perror("SHMAT");
        exit(1);
    }
    shm_ptr->p = 0;
    shm_ptr->c = 0;
    //SIMPLE FORKING FOR CHILD
    if(fork()==0)
    {
        producer();
        exit(0);
    }
    if(fork()==0)
    {
        consumer();
        exit(0);
    }
    // WAITING FOR THEM TO TERMINATE OR ELSE THEY WILL BECOME ZOMBIE PROCESS
    wait(NULL);
    wait(NULL);
    //FREE THE SHM_PTR
    shmdt(shm_ptr);
    // DEALLOCATE THE SHARED MEMORY BLOCK
    shmctl(shmid,IPC_RMID,NULL);
}
void producer()
{
    // SAME DESCRIPTION
    int shmid = shmget(SHM_KEY, sizeof(SHM_NEW), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("SHMGET");
        exit(1);
    }
    SHM_NEW *shm_ptr;
    shm_ptr = (SHM_NEW *)shmat(shmid, NULL, 0);
    if (shm_ptr == (SHM_NEW *)-1)
    {
        perror("SHMAT");
        exit(1);
    }

    // OPEN THE FILE ABC.txt WITH 
    // PERMISSION :0666
    //O_CREAT => CREATE THE FILE IF NOT EXISTS
    //O_TRUNC => EXISTING CONTENTS ARE REMOVED COMPLETELY AND SIZE IS SET TO 0
    int fd = open("ABC.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1)   
    {
        perror("FILE DESCRIPTOR");
        exit(1);
    }
    //WRITING A EMPTY SPACE AT THE END TO SPECIFY THE FILE SIZE OR ELSE ERROR MIGHT BE THERE 
    if (lseek(fd, BUFFER_SIZE - 1, SEEK_SET) == -1)
    {
        perror("LSEEK");
        exit(1);
    }
    if (write(fd, " ", 1) == -1)
    {
        perror("WRITE");
        exit(1);
    }
    // BASIC PRODUCER-CONSUMER LOGIC
    while (1)
    {
        while ((shm_ptr->p + 1) % BUFFER_SIZE == shm_ptr->c)
        {
            sleep(1);
        }

        lseek(fd, shm_ptr->p, SEEK_SET);

        char c = 'A' + rand() % 26;
        if (write(fd, &c, 1) == -1)
        {
            perror("WRITE");
            exit(1);
        }

        printf("WRITTEN: %c\n", c);

        shm_ptr->p = (shm_ptr->p + 1) % BUFFER_SIZE;
        sleep(1);
    }

    close(fd);
    shmdt(shm_ptr);
}

void consumer()
{
    char ch;
    int shmid = shmget(SHM_KEY, sizeof(SHM_NEW), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("SHMGET");
        exit(1);
    }
    SHM_NEW *shm_ptr;
    shm_ptr = (SHM_NEW *)shmat(shmid, NULL, 0);
    if (shm_ptr == (SHM_NEW *)-1)
    {
        perror("SHMAT");
        exit(1);
    }
    //ONLY READ AND WRITE PERMISSIONS
    int fd = open("ABC.txt", O_RDWR);
    if (fd == -1)
    {
        perror("FILE DESCRIPTOR");
        exit(1);
    }
    // BASIC PRODUCER-CONSUMER LOGIC
    while (1)
    {
        while (shm_ptr->p  == shm_ptr->c)
        {
            sleep(1);
        }
        lseek(fd, shm_ptr->c, SEEK_SET);
        if(read(fd,&ch,1)==-1)
        {
            perror("ERROR HERE");
            exit(1);
        }
        printf("CONSUMED: %c\n", ch);
        lseek(fd, shm_ptr->c, SEEK_SET);
        write(fd," ",1);
        shm_ptr->c = (shm_ptr->c + 1) % BUFFER_SIZE;

        sleep(1);
    }

    close(fd);
    shmdt(shm_ptr);
}