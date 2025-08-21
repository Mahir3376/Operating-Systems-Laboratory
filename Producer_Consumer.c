// Name: P.Mahir
// Roll No.: 22CSB0C27

// Solution to Producer-Consumer Problem

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;     // Indexes for producer (in) and consumer (out)
int empty, full, mutex;  // Semaphore IDs

// Initialize semaphore to a specified value using semop
void semaphore_initialize(int sem_id, int initial_value) {
    struct sembuf sop = {0, initial_value, 0};
    semop(sem_id, &sop, 1);  // Set initial value of semaphore
}

// Wait (P) operation
void wait(int sem_id) {
    struct sembuf sop = {0, -1, 0};
    semop(sem_id, &sop, 1);
}

// Signal (V) operation
void signal(int sem_id) {
    struct sembuf sop = {0, 1, 0};
    semop(sem_id, &sop, 1);
}

// Producer function to produce items and add them to the buffer
void* producer(void* arg) {
    int item;
    while (1) {
        item = rand() % 100;
        wait(empty);          // Wait if buffer is full
        wait(mutex);          // Locking the buffer for exclusive access

        buffer[in] = item;    // Insert item into buffer
        in = (in + 1) % BUFFER_SIZE; // Update buffer index for next item
        printf("Produced: %d\n", item);

        signal(mutex);
        signal(full);
        sleep(1);             // Simulate time taken to produce item
    }
}

// Consumer function to consume items from the buffer
void* consumer(void* arg) {
    int item;
    while (1) {
        wait(full);           // Wait if buffer is empty
        wait(mutex);          // Locking the buffer for exclusive access

        item = buffer[out];   // Remove item from buffer
        out = (out + 1) % BUFFER_SIZE; // Update index for next item
        printf("Consumed: %d\n", item);

        signal(mutex);
        signal(empty);
        sleep(1);
    }
}

int main() {
    pthread_t prod, cons;

    empty = semget(ftok("empty", 65), 1, 0666 | IPC_CREAT);
    full = semget(ftok("full", 66), 1, 0666 | IPC_CREAT);
    mutex = semget(ftok("mutex", 67), 1, 0666 | IPC_CREAT);

    semaphore_initialize(empty, BUFFER_SIZE);
    semaphore_initialize(full, 0);
    semaphore_initialize(mutex, 1);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Waiting for threads to finish
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}

