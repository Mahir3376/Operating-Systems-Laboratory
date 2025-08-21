// Name: P.Mahir
// Roll No.: 22CSB0C27

// Solution to Readers-Writers Problem

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

int readcount = 0;
int mutex, wrt;

// Initialize semaphore with an initial value
void semaphore_initialize(int sem_id, int initial_value) {
    struct sembuf sop = {0, initial_value, 0};
    semop(sem_id, &sop, 1);
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

// Reader function: multiple readers can read simultaneously
void* reader(void* arg) {
    while (1) {
        wait(mutex);
        readcount++;
        if (readcount == 1)       // First reader locks the writer
            wait(wrt);
        signal(mutex);            // Unlock readcount

        printf("Reader is reading\n"); // Critical section for reading
        sleep(1);                 // Simulate reading time

        wait(mutex);              // Lock access to readcount
        readcount--;
        if (readcount == 0)       // Last reader unlocks the writer
            signal(wrt);
        signal(mutex);            // Unlock readcount
        sleep(1);
    }
}

// Writer function: only one writer can write at a time
void* writer(void* arg) {
    while (1) {
        wait(wrt);               // Wait for exclusive access to write

        printf("Writer is writing\n"); // Critical section for writing
        sleep(1);               // Simulate writing time

        signal(wrt);            // Release exclusive access to write
        sleep(1);
    }
}

int main() {
    pthread_t r1, r2, w1;

    mutex = semget(ftok("mutex", 65), 1, 0666 | IPC_CREAT);
    wrt = semget(ftok("wrt", 66), 1, 0666 | IPC_CREAT);

    semaphore_initialize(mutex, 1);
    semaphore_initialize(wrt, 1);

    pthread_create(&r1, NULL, reader, NULL);
    pthread_create(&r2, NULL, reader, NULL);
    pthread_create(&w1, NULL, writer, NULL);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(w1, NULL);

    return 0;
}

