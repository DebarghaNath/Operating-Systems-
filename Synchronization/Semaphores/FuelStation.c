#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


#define CAR 5

int CarFuel[CAR] = {40, 30, 44, 24, 19};
int AvailFuel = 0,odd = 0;
pthread_mutex_t mutex,mutexFuel;
pthread_cond_t condFuel;


void * STATION(void * arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutexFuel);
        pthread_mutex_lock(&mutex);
        AvailFuel+=40;
        printf("FUELING IS DONE, AVAILABLE:%d \n",AvailFuel);
        odd = 1-odd;
        sleep(1);
        pthread_cond_broadcast(&condFuel);
        pthread_mutex_unlock(&mutex);
    }
}
void * Car(void * arg)
{
    int car = *(int *)arg;
    while(1)    
    {
        pthread_mutex_lock(&mutex);
        if((car%2)==(odd%2))
        {
            printf("CAR NO %d COMES FOR FUELING, AVAILABLE FUEL %d \n",car,AvailFuel);
            sleep(1);
            if(AvailFuel<CarFuel[car])
            {
                pthread_mutex_unlock(&mutexFuel);
            }
            while(AvailFuel<CarFuel[car])
            {
                pthread_cond_wait(&condFuel,&mutex);
            }
            AvailFuel-=CarFuel[car];
            printf("CAR NO %d DONE FUELING,  REMAINING FUEL %d \n",car,AvailFuel);
            sleep(1);
        }
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
}
int main()
{
    pthread_t car[CAR],station;
    int Car_id[CAR];
    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&mutexFuel,NULL);
    pthread_cond_init(&condFuel,NULL);
    pthread_mutex_lock(&mutexFuel);
    for(int i=0;i<CAR;i++)
    {
        Car_id[i] = i;
        pthread_create(&car[i],NULL,Car,(void *)&Car_id[i]);
    }
    pthread_create(&station,NULL,STATION,NULL);
    for(int i=0;i<CAR;i++)
    {
        pthread_join(car[i],NULL);
    }
    pthread_join(station,NULL);


    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);
}
