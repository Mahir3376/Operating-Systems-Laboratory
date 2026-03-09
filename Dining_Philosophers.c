// Solution to Dining Philosophers Problem

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

int forks[NUM_PHILOSOPHERS]; // Semaphore IDs for each fork

void semaphore_initialize(int sem_id, int initial_value) {
    struct sembuf sop = {0, initial_value, 0};
    semop(sem_id, &sop, 1);
}

void wait(int sem_id) {
    struct sembuf sop = {0, -1, 0};
    semop(sem_id, &sop, 1);
}

void signal(int sem_id) {
    struct sembuf sop = {0, 1, 0};
    semop(sem_id, &sop, 1);
}

// Philosopher alternates between thinking and eating
void* philosopher(void* num) {
    int i = *(int*)num;
    int left = i;
    int right = (i + 1) % NUM_PHILOSOPHERS;

    while (1) {
        printf("Philosopher %d is thinking\n", i);
        sleep(1);

        // Even philosophers pick left first, odd pick right
        if (i % 2 == 0) {
            wait(forks[left]);
            wait(forks[right]);
        } else {
            wait(forks[right]);
            wait(forks[left]);
        }

        printf("Philosopher %d is eating\n", i);
        sleep(1);

        signal(forks[left]);
        signal(forks[right]);
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Create and initialize fork semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        forks[i] = semget(ftok("forks", i), 1, 0666 | IPC_CREAT);
        semaphore_initialize(forks[i], 1);  // Each fork starts as available
        ids[i] = i;
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}

