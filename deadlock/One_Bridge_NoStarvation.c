#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Declare two threads for North and South farmers
pthread_t North, South;

// Declare a mutex to represent the bridge
pthread_mutex_t bridge;
pthread_mutex_t n, s;

// Declare a flag to enforce alternating crossing
int turn = 0;  // 0 for North, 1 for South

// Function for the North farmer's actions
void* NORTH(void* arg) {
    while(1) {
        // Lock the bridge mutex to check whose turn it is
        pthread_mutex_lock(&bridge);
        if(turn == 0) {  // Check if it's North's turn
            pthread_mutex_unlock(&n);  // Allow North to proceed
            turn = 1;  // Change turn to South
        }   
        pthread_mutex_unlock(&bridge);

        // Wait until North's turn and South has left the bridge
        pthread_mutex_lock(&n);
        
        // Lock bridge mutex to simulate crossing actions
        pthread_mutex_lock(&bridge);
        printf("North Farmer can cross ... \n");
        printf("North Farmer crossing....\n");

        // Simulate time taken to cross the bridge
        sleep(1);

        // Simulate the North farmer leaving the bridge
        printf("North Farmer left the Bridge...\n");
        
        // Allow South to proceed
        pthread_mutex_unlock(&s);
        pthread_mutex_unlock(&bridge);
        
        // Sleep briefly to prevent tight looping
        sleep(1);
    }
    return NULL;
}

// Function for the South farmer's actions
void* SOUTH(void* arg) {
    while(1) {
        // Lock the bridge mutex to check whose turn it is
        pthread_mutex_lock(&bridge);
        if(turn == 1) {  // Check if it's South's turn
            pthread_mutex_unlock(&s);  // Allow South to proceed
            turn = 0;  // Change turn to North
        }   
        pthread_mutex_unlock(&bridge);

        // Wait until South's turn and North has left the bridge
        pthread_mutex_lock(&s);
        
        // Lock bridge mutex to simulate crossing actions
        pthread_mutex_lock(&bridge);
        printf("South Farmer can cross ... \n");
        printf("South Farmer crossing....\n");

        // Simulate time taken to cross the bridge
        sleep(1);

        // Simulate the South farmer leaving the bridge
        printf("South Farmer left the Bridge...\n");
        
        // Allow North to proceed
        pthread_mutex_unlock(&n);
        pthread_mutex_unlock(&bridge);
        
        // Sleep briefly to prevent tight looping
        sleep(1);
    }
    return NULL;
}

int main() {
    // Initialize the mutex for the bridge and the farmer controls
    pthread_mutex_init(&bridge, NULL);
    pthread_mutex_init(&n, NULL);
    pthread_mutex_init(&s, NULL);

    // Lock the farmer controls to set initial turn conditions
    pthread_mutex_lock(&n);  // Initially lock North farmer
    pthread_mutex_lock(&s);  // Initially lock South farmer

    // Create threads for the North and South farmers
    pthread_create(&North, NULL, NORTH, NULL);
    pthread_create(&South, NULL, SOUTH, NULL);

    // Wait for both threads to finish (this won't actually happen in an infinite loop)
    pthread_join(North, NULL);
    pthread_join(South, NULL);

    // Destroy the mutex after use
    pthread_mutex_destroy(&bridge);
    pthread_mutex_destroy(&n);
    pthread_mutex_destroy(&s);

    return 0;
}
