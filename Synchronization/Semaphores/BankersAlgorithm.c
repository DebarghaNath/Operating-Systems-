#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define RESOURCES 4
#define PROCESSES 3

int max[PROCESSES][RESOURCES] = {{7, 5, 3, 2}, {3, 2, 2, 2}, {9, 0, 2, 2}};  
int available[RESOURCES] = {14, 6, 5, 5}; 
int request[PROCESSES][RESOURCES];   
int alloc[PROCESSES][RESOURCES];   

sem_t s[RESOURCES];  
int f[PROCESSES];    
int ans[PROCESSES];  
int exit_sequence[PROCESSES];
int ind = 0;
pthread_mutex_t mutex;

int banker_algo() 
{
    int work[RESOURCES];
    int finish[PROCESSES] = {0};
    int safe_sequence[PROCESSES];
    int idx = 0;

    for (int i = 0; i < RESOURCES; i++) 
    {
        work[i] = available[i];
    }

    printf("\nWORK ARRAY AT THE BEGINING: ");
    for (int i = 0; i < RESOURCES; i++) 
    {
        printf("%d ", work[i]);
    }
    printf("\n");

    
    int cnt = 0;
    for (int count = 0; count < PROCESSES; count++) 
    {    
        for (int p = 0; p < PROCESSES; p++) 
        {
            if (!finish[p]) 
            {
                int j;
                printf("\nCHECKING PROCESS %d: NEED: ", p + 1);
                for (j = 0; j < RESOURCES; j++) 
                {
                    int need = max[p][j] - alloc[p][j];
                    printf("%d ", need);
                    if (need > work[j])
                        break;
                }
                if (j == RESOURCES) 
                {
                     
                    printf("\nPROCESS %d CAN FINISH. WORK UPDATE: ", p + 1);
                    for (int k = 0; k < RESOURCES; k++) {
                        printf("%d ", work[k]);
                    }
                    safe_sequence[idx++] = p;
                    cnt++;
                    finish[p] = 1;
                    
                }
            }
        }
    }
    if(cnt!=PROCESSES)
    {
        return 0;
    }
    printf("\nSAFE SEQUENCE: ");
    for (int i = 0; i < PROCESSES; i++) 
    {
        printf("%d ", safe_sequence[i] + 1);
    }
    printf("\n");
    //sleep(1);
    return 1;
}

void *Process(void *index) 
{
    int i = *(int *)index;

    while (f[i] == 0) {  
        pthread_mutex_lock(&mutex);
        int flag = 1;
        printf("\nPROCESS %d'S ALLOCATION: ", i + 1);
        for (int j = 0; j < RESOURCES; j++) {
            int var = max[i][j] - alloc[i][j];  
            request[i][j] = rand() % (var + 1); 
            printf("%d ", alloc[i][j]);
            if (alloc[i][j] < max[i][j]) 
            {
                flag = 0;  
            }
        }
        printf("\nPROCESS %d'S REQUEST: ", i + 1);
        for (int j = 0; j < RESOURCES; j++) {
            printf("%d ", request[i][j]);
        }
        printf("\n");

        if (flag == 1) 
        {
            f[i] = 1;
            exit_sequence[ind++] = i+1;
            for(int j=0;j<RESOURCES;j++)
            {
                available[j]+= alloc[i][j];
            }
            printf("PROCESS %d HAS FINISHED EXECUTING.\n", i + 1);
            pthread_mutex_unlock(&mutex);
            return NULL;
        }


        int can_allocate = 1;

        for (int j = 0; j < RESOURCES; j++) 
        {
            if (request[i][j] > available[j])
            {
                can_allocate = 0;
                break;
            }
        }

        if (can_allocate) 
        {    
            for (int j = 0; j < RESOURCES; j++)
            {
                for(int k=0;k<request[i][j];k++)
                {
                    sem_wait(&s[j]);
                }
                available[j] -= request[i][j];
                alloc[i][j] += request[i][j];
            }

            if (banker_algo()) 
            {
                printf("PROCESS %d'S REQUEST IS GRANTED.\n", i + 1);
                pthread_mutex_unlock(&mutex);
            } 
            else 
            {
                for (int j = 0; j < RESOURCES; j++) 
                {
                    for(int k=0;k<request[i][j];k++)
                    {
                        sem_post(&s[j]);
                    }
                    available[j] += request[i][j];
                   
                }
                printf("PROCESS %d'S REQUEST IS DENIED (unsafe state).\n", i + 1);
                pthread_mutex_unlock(&mutex);
                sleep(1);
                continue;
            }
        } 
        else 
        {
            for(int j=0;j<RESOURCES;j++)
            {
                for(int k=0;k<request[i][j];k++)
                {
                    sem_post(&s[j]);
                }
            }
            printf("PROCESS %d IS WAITING FOR RESOURCES.\n", i + 1);
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
    for (int i = 0; i < RESOURCES; i++) 
    {
        sem_init(&s[i], 0, available[i]);
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_t process[PROCESSES];
    int process_id[PROCESSES];

    for (int i = 0; i < PROCESSES; i++) 
    {
        f[i] = 0;
        for (int j = 0; j < RESOURCES; j++) 
        {
            alloc[i][j] = 0;
        }
    }

    for (int i = 0; i < PROCESSES; i++) 
    {
        process_id[i] = i;
        pthread_create(&process[i], NULL, Process, (void *)&process_id[i]);
    }

    for (int i = 0; i < PROCESSES; i++) 
    {
        pthread_join(process[i], NULL);
    }

    int safe = 1;
    for (int i = 0; i < PROCESSES; i++) 
    {
        if (f[i] == 0) 
        {
            safe = 0;  
            break;
        }
    }

    if (safe) 
    {
        printf("ALL PROCESS COMPLETED SAFELY.\n");
        printf("EXIT SEQUENCE IS: \n");
        for(int i=0;i<PROCESSES;i++)
        {
            printf("%d=>",exit_sequence[i]);
        }
        printf("\n");
    } 
    else 
    {
        printf("NOT ALL PROCESS COULD COMPLETE UNSAFE STATE.\n");
    }

    for (int i = 0; i < RESOURCES; i++) 
    {
        sem_destroy(&s[i]);
    }

    return 0;
}
