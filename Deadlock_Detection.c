// Name: P.Mahir
// Roll No.: 22CSB0C27

// Deadlock Detection Algorithm

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

int numProcesses = 5;
int numResources = 3;

int Allocation[5][3] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 3},
    {2, 1, 1},
    {0, 0, 2}
};

int Request[5][3] = {
    {0, 0, 0},
    {2, 0, 2},
    {0, 0, 1},
    {1, 0, 0},
    {0, 0, 2}
};

int Available[3] = {0, 0, 0};
bool Finish[5] = {false};  // Track finished processes

pthread_mutex_t mutex;

// Function to check if a process can be allocated the resources it requests
bool canAllocate(int i, int Work[]) {
    for (int j = 0; j < numResources; j++) {
        if (Request[i][j] > Work[j]) {
            return false;  // If request exceeds current resources, allocation fails
        }
    }
    return true;
}

// Thread function for each process to check for deadlock
void* processThread(void* arg) {
    int processID = *(int*)arg;
    int Work[3];

    // Copy available resources to local Work array
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < numResources; i++) {
        Work[i] = Available[i];
    }
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    if (!Finish[processID] && canAllocate(processID, Work)) {
        // If the process can complete, mark as finished and update Work
        for (int j = 0; j < numResources; j++) {
            Work[j] += Allocation[processID][j];  // Release allocated resources
        }
        Finish[processID] = true;
        printf("Process %d can complete and is marked as finished.\n", processID);
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

// Function to detect deadlock by creating threads for each process
void detect_Deadlock() {
    pthread_t threads[numProcesses];
    int processIDs[numProcesses];

    // Create a thread for each process
    for (int i = 0; i < numProcesses; i++) {
        processIDs[i] = i;
        pthread_create(&threads[i], NULL, processThread, &processIDs[i]);
    }

    // Join each thread after completion
    for (int i = 0; i < numProcesses; i++) {
        pthread_join(threads[i], NULL);
    }

    // Check Finish array to identify any deadlocked processes
    bool deadlockDetected = false;
    for (int i = 0; i < numProcesses; i++) {
        if (!Finish[i]) {
            printf("Process %d is deadlocked.\n", i);
            deadlockDetected = true;
        }
    }

    if (!deadlockDetected) {
        printf("No deadlock detected.\n");
    }
}

int main() {
    // Initialize mutex for resource protection
    pthread_mutex_init(&mutex, NULL);

    detect_Deadlock();

    // Destroy mutex after all threads have finished
    pthread_mutex_destroy(&mutex);

    return 0;
}
