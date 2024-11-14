#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Declare two threads for North and South farmers
pthread_t North, South;

// Declare a mutex to represent the bridge
pthread_mutex_t bridge;

// Function for the North farmer's actions
void NORTH()
{
    while(1)
    {
        // Lock the mutex to ensure mutual exclusion on the bridge
        pthread_mutex_lock(&bridge);
        
        // Simulate the North farmer crossing the bridge
        printf("North Farmer can cross ... \n");
        printf("North Farmer crossing....\n");
        
        // Simulate time taken to cross the bridge (random sleep between 0-3 seconds)
        sleep((rand()%4));
        
        // Simulate the North farmer leaving the bridge
        printf("North Farmer left the Bridge...\n");
        
        // Unlock the mutex after the farmer crosses
        pthread_mutex_unlock(&bridge);
        sleep(rand()%4);
    }
}

// Function for the South farmer's actions
void SOUTH()
{
    while(1)
    {
        // Lock the mutex to ensure mutual exclusion on the bridge
        pthread_mutex_lock(&bridge);
        
        // Simulate the South farmer crossing the bridge
        printf("South Farmer can cross ... \n");
        printf("South Farmer crossing....\n");
        
        // Simulate time taken to cross the bridge (random sleep between 0-3 seconds)
        sleep((rand()%4));
        
        // Simulate the South farmer leaving the bridge
        printf("South Farmer left the Bridge...\n");
        
        // Unlock the mutex after the farmer crosses
        pthread_mutex_unlock(&bridge);
        sleep(rand()%4);
    }
}

int main()
{
    // Initialize the mutex for the bridge
    pthread_mutex_init(&bridge, NULL);

    // Create threads for the North and South farmers
    pthread_create(&North, NULL, (void*)NORTH, NULL);
    pthread_create(&South, NULL, (void*)SOUTH, NULL);

    // Wait for both threads to finish (this won't actually happen in an infinite loop)
    pthread_join(North, NULL);
    pthread_join(South, NULL);

    // Destroy the mutex after use
    pthread_mutex_destroy(&bridge);

    return 0;
}
