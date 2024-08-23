/*
    when a thread is created some part of memory is allocated from parent process memory block to the threads. So they share resources
    means synchornization error might occur due to data inconsistency.
    **EXAMPLE of synchronization issue called race condition** 

    int counter = 0;// trigger for race condition
    void * count_big(void * arg)
    {
        for(int i = 0;i<=1e9;i++)
        {
            counter++;
        }
        return NULL;
    }
    int main()
    {
        pthread_t t;
        pthread_create(&t,NULL,count_big,NULL);
        count_big(NULL);
        printf("counter: %d",counter);
        return 0;   
    }
    two thread simultaneously update the same counter variable leads to data inconsistency
*/

// solution Mutex lock (mutual exclusion) locks


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // initialize a variable lock
void * count_big(void * arg)
{
    for(int i = 0;i<=1e9;i++)
    {
        pthread_mutex_lock(&lock); //critical_section 
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
int main()
{
    pthread_t t;
    pthread_create(&t,NULL,count_big,NULL);
    count_big(NULL);
    printf("counter: %d",counter);
    return 0;   
}

/*
    interesting to note using thread here increases the time of execution which is counter intuitive.
    beacaues of the mutex_lock calls in this code which slows the program
*/