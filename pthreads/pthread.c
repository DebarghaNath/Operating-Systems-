#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
/*
    (a+b) + (c+d)
    one thread will do (a+b)
    and other will do(c+d)
    and in main addition will be done
*/ 
/*
    defining a struct for passing as argument
*/
typedef struct
{
    int a;
    int b;
} pair;

// void * return type adder function 
void * Add(void * arg)
{
    // type cast the arg into pair
    pair * newpair = (pair *)arg;
    // for safe return type in thread dynamic memory allocation is better
    int *val = (int * )malloc(sizeof(int));
    *val = newpair->a+newpair->b;
    //return void* type 
    return (void*) val;
}

int main()
{
    pthread_t newthread1,newthread2;
    //pthread_t newthread stores a pointer to the new thread of function created 
    pair p1,p2;
    p1.a = 10;
    p1.b = 20;
    p2.a = 10;
    p2.b = 20;
    pthread_create(&newthread1,NULL,Add,&p1);
    pthread_create(&newthread2,NULL,Add,&p2);
    int *res1,*res2;
    pthread_join(newthread1,(void *)&res1);
    pthread_join(newthread2,(void *)&res2);
    printf("ANSWER IS: %d",*res1+*res2);
}