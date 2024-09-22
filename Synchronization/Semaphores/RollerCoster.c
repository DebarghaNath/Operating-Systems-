/*
    Synchronization: RollerCoster Ride
    Author: Debargha Nath
    Question:Consider a roller coaster ride at an amusement park. The ride operator runs the ride only when
    there are exactly N riders on it. Multiple riders arrive at the ride and queue up at the entrance of
    the ride. The ride operator waits for N riders to accumulate, and may even take a nap as he waits.
    Once N riders have arrived, the riders call out to the operator indicating they are ready to go on
    the ride. The operator then opens the gate to the ride and signals exactly N riders to enter the ride.
    He then waits until these N riders enter the ride, and then proceeds to start the ride
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define RIDER 10
#define LIMIT 5


sem_t mutex,operator;
sem_t cus[RIDER];
int count = LIMIT;
int ride[LIMIT];

void * Operator()
{
    sem_wait(&operator);
    printf("RIDE IS OPERATING................\n");
    sleep(2);
    sem_wait(&mutex);
    count = LIMIT;
    sem_post(&mutex);
    for(int i=0;i<LIMIT;i++)
    {
        printf("RIDER ID %d IS LEAVING THE RIDE \n",ride[i]);
        sem_post(&cus[ride[i]]);
        sleep(2);
    }
}
void * customer(void * arg)
{
   int id = *(int*)arg; 
    while(1)
    {
        sem_wait(&mutex);
        if(count>0)
        {
            count--;
            
            ride[count] = id;
            printf("RIDER %d IS ENTERING THE RIDE \n",id);
            sleep(2);
            if(count==0)
            {
                sem_post(&operator);
            }
            sem_post(&mutex);
            sem_wait(&cus[id]);
        }
        else
        {
            sem_post(&mutex);
        }
        sleep(2);
        printf("RIDER %d IS WAITING TO ENTER THE RIDE \n",id);
        sleep(2);

    }
}
int main(int argc, char *argv[])
{
    pthread_t Customer[RIDER];
    pthread_t op;
    int rider_ids[RIDER]; 
    sem_init(&mutex, 0,1);
    sem_init(&operator,0,0);
    for(int i=0;i<RIDER;i++)
    {
        sem_init(&cus[i],0,0);
    }
    for(int i=0;i<RIDER;i++)
    {
        rider_ids[i] = i;
        pthread_create(&Customer[i],NULL,customer,(void *)&rider_ids[i]);
    }
    pthread_create(&op,NULL,Operator,NULL);
    for(int i=0;i<RIDER;i++)
    {
        pthread_join(Customer[i],NULL);
    }
    pthread_join(op,NULL);
}

