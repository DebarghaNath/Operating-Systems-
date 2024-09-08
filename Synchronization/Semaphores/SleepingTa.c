#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STUDENT 5
#define Chair 3

sem_t semaphore_ta;  // Semaphore to notify the TA
sem_t semaphore_stu; // Semaphore to notify students
sem_t mutex;         // Mutex to protect shared resources

int chair[Chair];    // Array to hold student IDs in waiting chairs
int ind = 0;       // Number of students currently in chairs
int next_seat = 0;   // Index of the next available seat
int next_help = 0;   // Index of the next student to help

void *TA_Process(void *arg)
{
    while (1)
    {
        sem_wait(&semaphore_ta);   // Wait for a student to notify
        sem_wait(&mutex);          // Enter critical section

        int student = chair[next_help]; // Get the student from the chair
        next_help = (next_help + 1) % Chair; // Move to the next student
        ind--;  // Decrease the number of students waiting

        sem_post(&mutex);          // Exit critical section
        printf("THE TA IS HELPING STUDENT %d\n", student);
        sleep(rand() % 5 + 1);     // Simulate helping time
        printf("THE TA FINISHED HELPING STUDENT %d\n", student);

        sem_post(&semaphore_stu);  // Notify the student that help is done
    }
}

void *STUDENT_Process(void *arg)
{
    int id = *(int *)arg;  // Get student ID
    while (1)
    {
        sleep(rand() % 5 + 1);  // Simulate programming time

        sem_wait(&mutex);  // Enter critical section
        if (ind < Chair)
        {
            chair[next_seat] = id;       // Sit in the next available chair
            next_seat = (next_seat + 1) % Chair;  // Update the next available seat
            ind++;  // Increase the number of students waiting
            printf("STUDENT %d WANTS HELP\n", id);

            sem_post(&semaphore_ta);  // Notify TA that a student needs help
            sem_post(&mutex);         // Exit critical section

            sem_wait(&semaphore_stu);  // Wait until TA finishes helping
            printf("STUDENT %d HAS BEEN HELPED\n", id);
        }
        else
        {
            sem_post(&mutex);  // Exit critical section if no chairs are available
            printf("STUDENT %d FOUND NO AVAILABLE CHAIR AND WILL TRY AGAIN LATER\n", id);
        }
    }
}

int main()
{
    pthread_t TA, STUDENT[MAX_STUDENT];
    sem_init(&semaphore_ta, 0, 0);  // Initialize TA semaphore
    sem_init(&semaphore_stu, 0, 0); // Initialize student semaphore
    sem_init(&mutex, 0, 1);         // Initialize mutex

    pthread_create(&TA, NULL, TA_Process, NULL);  // Create TA thread

    int ids[MAX_STUDENT];
    for (int i = 0; i < MAX_STUDENT; i++)
    {
        ids[i] = i + 1;  // Assign student IDs
        pthread_create(&STUDENT[i], NULL, STUDENT_Process, (void *)&ids[i]);  // Create student threads
    }

    pthread_join(TA, NULL);  // Join TA thread (although in this case it runs indefinitely)

    for (int i = 0; i < MAX_STUDENT; i++)
    {
        pthread_join(STUDENT[i], NULL);  // Join student threads
    }

    // Destroy semaphores (not reachable due to infinite loops but good practice)
    sem_destroy(&semaphore_ta);
    sem_destroy(&semaphore_stu);
    sem_destroy(&mutex);

    return 0;
}
