/* 
    Author: Debargha Nath
    Date: 29/08/2024
    
    Problem Statement:
        The Dining Philoshoper Problem states that K philoshopers are seated around a circular table with one chopstick between each pair of philoshoper.
        There is one chopstick between each philoshopers. A philoshoper may eat if hea can pick up two chopstick adjacent to him.
    Solution:
        Critical Section is when a Philoshoper picks up two chopstick to eat(Use hardware synchronization to protect that block) Semaphores
        make 3 state for each Philoshoper:
        eating: pickup 2 adjacent chopsticks
        thinking: putdown 2 adjacent chopsticks
        hungry: wait for 2 adjacnt chopstick to be free
*/
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define N 5
#define Hungry 2
#define Eating 1
#define Thinking 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

int state[N];
int phil[N] = {0, 1, 2, 3, 4};

sem_t mutex;
sem_t S[N];

void check(int phnum) 
{
    // Check if the philosopher can start eating
    if (state[phnum] == Hungry && state[LEFT] != Eating && state[RIGHT] != Eating) {
        state[phnum] = Eating;
        printf("Philosopher %d takes chopsticks %d and %d\n", phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is Eating\n", phnum + 1);
        sem_post(&S[phnum]);
    }
    
}

void pick_fork(int phnum) {
    sem_wait(&mutex);

    // Mark philosopher as hungry and try to pick up chopsticks
    state[phnum] = Hungry;
    printf("Philosopher %d is Hungry\n", phnum + 1);

    check(phnum);

    sem_post(&mutex);
    sem_wait(&S[phnum]); // Wait until the philosopher is allowed to eat

    sleep(1);
}

void put_fork(int phnum) {
    sem_wait(&mutex);

    // Mark philosopher as thinking and put down chopsticks
    state[phnum] = Thinking;
    printf("Philosopher %d puts down chopsticks %d and %d\n", phnum + 1, LEFT + 1, phnum + 1);

    // Check if the left and right philosophers can now eat
    check(LEFT);
    check(RIGHT);

    sem_post(&mutex);
}

void *simulate(void *arg) {
    int *id = (int *)arg;
    while (1) {
        sleep(1);
        pick_fork(*id);
        sleep(0); // Simulating eating
        put_fork(*id);
    }
}

int main() {
    pthread_t thread[N];

    // Initialize the semaphores
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < N; i++) {
        sem_init(&S[i], 0, 0);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        pthread_create(&thread[i], NULL, simulate, &phil[i]);
        printf("Philosopher %d is thinking\n", phil[i] + 1);
    }

    // Join the threads
    for (int i = 0; i < N; i++) {
        pthread_join(thread[i], NULL);
    }

    return 0;
}

