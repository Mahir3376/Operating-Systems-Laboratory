// Name: P.Mahir
// Roll No.: 22CSB0C27

// Solution to Sleeping Barber Problem

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define NUM_CHAIRS 3  // Number of waiting chairs

int waiting = 0;       // Current number of waiting customers
int mutex, customers, barber;

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

// Customer waits for service or leaves if no chairs available
void* customer(void* arg) {
    while (1) {
        wait(mutex); // Lock for waiting counter
        if (waiting < NUM_CHAIRS) {
            waiting++;
            printf("Customer waiting. Total waiting: %d\n", waiting);
            signal(customers);
            signal(mutex);
            wait(barber);
            printf("Customer getting haircut\n");
        } else {
            printf("No chairs available. Customer leaving.\n");
            signal(mutex);
        }
        sleep(1); // Time before next customer arrives
    }
}

// Barber serves customers or sleeps if no one is waiting
void* barber_function(void* arg) {
    while (1) {
        wait(customers);  // Wait for a customer
        wait(mutex);

        waiting--;        // Decrease waiting count
        printf("Barber cutting hair. Waiting customers: %d\n", waiting);

        signal(barber);   // Serve a customer
        signal(mutex);    // Release waiting lock

        sleep(2);         // Time taken for a haircut
    }
}

int main() {
    pthread_t b, c[5];

    mutex = semget(ftok("mutex", 65), 1, 0666 | IPC_CREAT);
    customers = semget(ftok("customers", 66), 1, 0666 | IPC_CREAT);
    barber = semget(ftok("barber", 67), 1, 0666 | IPC_CREAT);

    semaphore_initialize(mutex, 1);
    semaphore_initialize(customers, 0); // Initially no customers waiting
    semaphore_initialize(barber, 0);    // Barber initially sleeping

    pthread_create(&b, NULL, barber_function, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_create(&c[i], NULL, customer, NULL);
    }

    pthread_join(b, NULL);
    for (int i = 0; i < 5; i++) {
        pthread_join(c[i], NULL);
    }

    return 0;
}

