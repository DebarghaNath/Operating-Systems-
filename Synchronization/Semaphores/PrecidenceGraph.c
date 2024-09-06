/* 
    Author: Debargha Nath
    Date: 6/09/2024


    Precidence Graph using semaphores
                     (1)
                    /   \
                   /    \
                 (2)   (3)
                   \    /
                   \   /
                   \  /
                   (4)
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

// semaphores 
sem_t a1,a2,a3,a4;

int cnt = 0;
void * process1(void * arg)
{   
    while(1)
    {
        sem_wait(&a1);  
        sleep(1);
        printf("PROCESS 1 \n");
        sleep(1);
        sem_post(&a2);
        sem_post(&a3);
        // process 1 is locked means only after complete execution 1 will run again
        // process 2 lock is released
        // process 3 lock is released
    }
    return NULL;
}
void * process4(void * arg)
{   
    while(1)
    {
        if(cnt==2)
        {
            // accquire lock only if both 2 and 3 releases
            sem_wait(&a4);
            sleep(1);
            printf("PROCESS 4 \n");
            sleep(1);
            sem_post(&a1);
            // release lock for process1
            cnt = 0;

        }
    }
    return NULL;
}
void * process2(void * arg)
{   
    while(1)
    {
        sem_wait(&a2);
        sleep(1);
        printf("PROCESS 2 \n");
        sleep(1);
        cnt++;
        sem_post(&a4);
        // unlock process 4 while keeping a counter
    }
    return NULL;
}
void * process3(void * arg)
{   
    while(1)
    {
        sem_wait(&a3);
        sleep(1);
        printf("PROCESS 3 \n");
        sleep(1);
        cnt++;
        sem_post(&a4);
        // same as process 2
    }
    return NULL;
}

int main()
{
    sem_init(&a1,0,1);
    sem_init(&a2,0,0);
    sem_init(&a3,0,0);
    sem_init(&a4,0,0);//process 2,3,4 are locked 
    pthread_t p1,p2,p3,p4;
    pthread_create(&p1,NULL,process1,NULL);
    pthread_create(&p2,NULL,process2,NULL);
    pthread_create(&p1,NULL,process3,NULL);
    pthread_create(&p2,NULL,process4,NULL);

    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    pthread_join(p3,NULL);
    pthread_join(p4,NULL);

    return 0;
}
