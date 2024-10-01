#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM_RESOURCES 4
#define NUM_PROCESSES 3

sem_t s[NUM_RESOURCES];  
int f[NUM_PROCESSES];    
int ans[NUM_PROCESSES];  
int max[NUM_PROCESSES][NUM_RESOURCES] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}};  
int available[NUM_RESOURCES] = {5, 5, 5, 5};  
int request[NUM_PROCESSES][NUM_RESOURCES];   
int alloc[NUM_PROCESSES][NUM_RESOURCES];     
pthread_mutex_t mutex;

int is_safe() {
    int work[NUM_RESOURCES];
    int finish[NUM_PROCESSES] = {0};
    int safe_sequence[NUM_PROCESSES];
    int idx = 0;

    for (int i = 0; i < NUM_RESOURCES; i++) {
        work[i] = available[i];
    }

    printf("\nWork array at start: ");
    for (int i = 0; i < NUM_RESOURCES; i++) {
        printf("%d ", work[i]);
    }
    printf("\n");

    int found = 0;
    for (int count = 0; count < NUM_PROCESSES; count++) {
        
        for (int p = 0; p < NUM_PROCESSES; p++) {
            if (!finish[p]) {
                int j;
                printf("\nChecking Process %d: Need: ", p + 1);
                for (j = 0; j < NUM_RESOURCES; j++) {
                    int need = max[p][j] - alloc[p][j];
                    printf("%d ", need);
                    if (need > work[j])
                        break;
                }
                if (j == NUM_RESOURCES) {
                    for (int k = 0; k < NUM_RESOURCES; k++)
                        work[k] += alloc[p][k];  
                    printf("\nProcess %d can finish. Work updated: ", p + 1);
                    for (int k = 0; k < NUM_RESOURCES; k++) {
                        printf("%d ", work[k]);
                    }
                    safe_sequence[idx++] = p;
                    finish[p] = 1;
                    found = 1;
                }
            }
        }
        if (!found) {
            return 0;  
        }
    }

    printf("\nSafe sequence: ");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("%d ", safe_sequence[i] + 1);
    }
    printf("\n");

    return 1;
}

void *Process(void *index) {
    int i = *(int *)index;

    while (f[i] == 0) {  
        pthread_mutex_lock(&mutex);
        int flag = 1;
        printf("\nProcess %d's Allocation: ", i + 1);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            int var = max[i][j] - alloc[i][j];  
            request[i][j] = rand() % (var + 1); 
            printf("%d ", alloc[i][j]);
            if (alloc[i][j] < max[i][j]) {
                flag = 0;  
            }
        }
        printf("\nProcess %d's Request: ", i + 1);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            printf("%d ", request[i][j]);
        }
        printf("\n");

        if (flag == 1) { 
            f[i] = 1;
            printf("Process %d has finished executing.\n", i + 1);
            pthread_mutex_unlock(&mutex);
            return NULL;
        }


        int can_allocate = 1;
        for (int j = 0; j < NUM_RESOURCES; j++) {
            if (request[i][j] > available[j]) {
                can_allocate = 0;
                break;
            }
        }

        if (can_allocate) {
            
            for (int j = 0; j < NUM_RESOURCES; j++) {
                available[j] -= request[i][j];
                alloc[i][j] += request[i][j];
            }

            if (is_safe()) {
                printf("Process %d's request is granted.\n", i + 1);
                pthread_mutex_unlock(&mutex);
            } else {
                for (int j = 0; j < NUM_RESOURCES; j++) {
                    available[j] += request[i][j];
                    alloc[i][j] -= request[i][j];
                }
                printf("Process %d's request is denied (unsafe state).\n", i + 1);
                pthread_mutex_unlock(&mutex);
                sleep(1);
                continue;
            }
        } else {
            printf("Process %d is waiting for resources.\n", i + 1);
            pthread_mutex_unlock(&mutex);
            sleep(1);
            continue;
        }

        sleep(1);
    }

    return NULL;
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < NUM_RESOURCES; i++) {
        sem_init(&s[i], 0, available[i]);
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_t process[NUM_PROCESSES];
    int process_id[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; i++) {
        f[i] = 0;
        for (int j = 0; j < NUM_RESOURCES; j++) {
            alloc[i][j] = 0;
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        process_id[i] = i;
        pthread_create(&process[i], NULL, Process, (void *)&process_id[i]);
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        pthread_join(process[i], NULL);
    }

    int safe = 1;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (f[i] == 0) {
            safe = 0;  
            break;
        }
    }

    if (safe) {
        printf("All processes completed safely.\n");
    } else {
        printf("Not all processes could complete; deadlock occurred.\n");
    }

    for (int i = 0; i < NUM_RESOURCES; i++) {
        sem_destroy(&s[i]);
    }

    return 0;
}
