// Name: P.Mahir
// Roll No.: 22CSB0C27

// Deadlock Avoidance using Banker's Algorithm

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_PROCESSES 5
#define NUM_RESOURCES 3

int allocation[NUM_PROCESSES][NUM_RESOURCES] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
};

int maxMatrix[NUM_PROCESSES][NUM_RESOURCES] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};

int available[NUM_RESOURCES] = {3, 3, 2};
int need[NUM_PROCESSES][NUM_RESOURCES];
pthread_mutex_t mutex;  // Mutex for resource allocation

// Calculate the need matrix
void calculateNeed() {
    for (int i = 0; i < NUM_PROCESSES; i++) {
        for (int j = 0; j < NUM_RESOURCES; j++) {
            need[i][j] = maxMatrix[i][j] - allocation[i][j];
        }
    }
}

// Check if the system is in a safe state
bool isSafe() {
    int work[NUM_RESOURCES];
    bool finish[NUM_PROCESSES] = {false};
    int safeSequence[NUM_PROCESSES];
    int count = 0;

    // Initialize work array with available resources
    for (int i = 0; i < NUM_RESOURCES; i++) {
        work[i] = available[i];
    }

    // Find a process that can finish with current work resources
    while (count < NUM_PROCESSES) {
        bool found = false;
        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < NUM_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                // If process can complete, mark it and adjust work
                if (canAllocate) {
                    for (int j = 0; j < NUM_RESOURCES; j++) {
                        work[j] += allocation[i][j];
                    }
                    safeSequence[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }

        // If no process could proceed, system is unsafe
        if (!found) {
            return false;
        }
    }

    return true;
}

// Request resources for a process
bool requestResources(int processID, int request[]) {
    pthread_mutex_lock(&mutex);

    // Check if request exceeds the process's need
    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (request[i] > need[processID][i]) {
            pthread_mutex_unlock(&mutex);
            return false;
        }
    }

    // Check if request exceeds available resources
    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (request[i] > available[i]) {
            pthread_mutex_unlock(&mutex);
            return false;
        }
    }

    // Temporarily allocate resources to process
    for (int i = 0; i < NUM_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[processID][i] += request[i];
        need[processID][i] -= request[i];
    }

    bool safe = isSafe();  // Check if state is safe

    // If unsafe, roll back allocation
    if (!safe) {
        for (int i = 0; i < NUM_RESOURCES; i++) {
            available[i] += request[i];
            allocation[processID][i] -= request[i];
            need[processID][i] += request[i];
        }
    }

    pthread_mutex_unlock(&mutex);
    return safe;
}

// Thread function for each process to make resource requests
void* processThread(void* arg) {
    int processID = *(int*)arg;
    int request[NUM_RESOURCES] = {1, 0, 2};  // Just an example request

    if (requestResources(processID, request)) {
        printf("Process %d's request granted.\n", processID);
    } else {
        printf("Process %d's request denied.\n", processID);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_PROCESSES];
    int processIDs[NUM_PROCESSES];

    // Initialize mutex lock
    pthread_mutex_init(&mutex, NULL);

    // Calculate need matrix based on maxMatrix and allocation
    calculateNeed();

    // Create threads for each process
    for (int i = 0; i < NUM_PROCESSES; i++) {
        processIDs[i] = i;
        pthread_create(&threads[i], NULL, processThread, &processIDs[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy mutex lock
    pthread_mutex_destroy(&mutex);

    return 0;
}
