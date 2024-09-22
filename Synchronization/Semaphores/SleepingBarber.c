/*
    Author: Debargha Nath
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define CHAIR 3
#define LIMIT 5

int count=0;
int chair[CHAIR];
sem_t mutex,mutex_barber;
sem_t mutex_customer[LIMIT];
void * Barber()
{
    while(1)
    {
        sem_wait(&mutex_barber);
        sem_wait(&mutex);
        int cus = chair[0];
        count--;
        for(int i=1;i<CHAIR;i++)
        {
            chair[i] = chair[i+1];
        }
        sem_post(&mutex);
        sem_post(&mutex_customer[cus]);
        sleep(2);
        printf("BARBER IS CUTTING THE HAIR OF CUSTOMER %d \n",cus);
        sleep(2);
    }

}
void * Customer(void * index)
{
    int id = *(int*)index;
    while(1)
    {
        sem_wait(&mutex);
        if(count<CHAIR-1)
        {
            chair[count]=id;
            count++;
            sleep(2);
            printf("CUSTOMER %d SITS ON A CHAIR\n", id);
            sleep(2);
            sem_post(&mutex_barber);
            sem_post(&mutex);
            sem_wait(&mutex_customer[id]);
        }
        else
        {
            sem_post(&mutex);
        }
        
        printf("CUSTOMER %d IS WAITING FOR CHAIR \n",id);
        sleep(2
        );
    }   
}
int main(int argc,char *argv[])
{
    pthread_t customer[LIMIT];
    pthread_t barber;
    sem_init(&mutex,0,1);
    sem_init(&mutex_barber,0,0);
    int customer_id[LIMIT];
    for(int i=0;i<LIMIT;i++)
    {
        sem_init(&mutex_customer[i],0,0);
    }
    for(int i=0;i<LIMIT;i++)
    {
        customer_id[i] = i;
        pthread_create(&customer[i],NULL,Customer,(void*)&customer_id[i]);
    }
    pthread_create(&barber,NULL,Barber,NULL);
    for(int i=0;i<LIMIT;i++)
    {
        pthread_join(customer[i],NULL);
    }
    pthread_join(barber,NULL);
}
