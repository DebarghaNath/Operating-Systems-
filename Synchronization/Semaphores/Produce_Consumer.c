/*
    Author: Debargha Nath
    Date: 29/08/2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>   
#include <semaphore.h>
#include <unistd.h>   
#include <pthread.h>  

#define BUFFER_SIZE 100

sem_t S;
int count = 0;  

void *produce(void *arg) {   
    int fd = open("ABC.c", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("Error opening file for writing");
        pthread_exit(NULL);
    }
    while (1) {
        sem_wait(&S); 
        if (count < BUFFER_SIZE) {
            lseek(fd, count, SEEK_SET); 
            char c = 'A' + rand() % 26; 
            write(fd, &c, sizeof(c));    
            printf("WRITTEN IN FILE %c AT LOCATION %d\n", c, count);
            count = (count + 1) % BUFFER_SIZE;
        }
        sem_post(&S); 
        sleep(1);  
    }

    close(fd);
    return NULL;
}

void *consume(void *arg) {
    int fd = open("ABC.c", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        pthread_exit(NULL);
    }
    while (1) {
        sleep(2);
        sem_wait(&S); 
        if (count > 0) {
            lseek(fd, count - 1, SEEK_SET); 
            char c;
            read(fd, &c, sizeof(c));     
            printf("READ FROM FILE %c AT LOCATION %d\n", c, count - 1);
            count = (count - 1) % BUFFER_SIZE;
        }
        sem_post(&S);  
        sleep(2); 
    }

    close(fd);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    sem_init(&S, 0, 1);

    pthread_create(&t1, NULL, produce, NULL);
    pthread_create(&t2, NULL, consume, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&S);  
    return 0;
}
