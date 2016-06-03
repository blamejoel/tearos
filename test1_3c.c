#include "types.h"
#include "user.h"
#include "semaphore.h"
#define TEST_NUM 21

int numCan, numMiss;
Semaphore mutex, cannibal, missionary, row;

void RowBoat() {
    printf(1, "Rowing!\n");
}

int num_threads;

void MissionaryArrives(void *arg); 
void CannibalArrives(void *arg); 


int main() {

// like this

    numCan = numMiss = num_threads = 0;

    num_threads = 0;
    sem_init(&mutex, 1);
    sem_init(&cannibal, 0);
    sem_init(&missionary, 0);
    sem_init(&row, 0);

    int i;
    int *tid;

    printf(1, "Testing with %d randomly generated canibals and missionaries\n", TEST_NUM);
    for (i = 1; i < TEST_NUM+1; i++) {
        if ((random(99999*i)/101)%2) {
            tid = thread_create(CannibalArrives, (void*)0);
            if (tid == 0) {
                printf(1, "failed to create thread!\n");
                exit();
            }
            sem_acquire(&mutex);
            num_threads++;
            sem_signal(&mutex);
        }
        else {
            tid = thread_create(MissionaryArrives, (void*)0);
            if (tid == 0) {
                printf(1, "failed to create thread!\n");
            }
            sem_acquire(&mutex);
            num_threads++;
            sem_signal(&mutex);
        }
    }
    for (i = 0; i < 3; i++){
        tid = thread_create(MissionaryArrives, (void*)0);
        if (tid == 0) {
            printf(1, "failed to create thread!\n");
        }
        sem_acquire(&mutex);
        num_threads++;
        sem_signal(&mutex);
    }

    printf(1, "exiting!");
    while(wait()>0);
    exit();
}
void CannibalArrives(void *arg) {
    sem_acquire(&mutex);
    if (numCan >= 2) {
        numCan -= 2;
        sem_signal(&mutex);
        sem_signal(&cannibal);
        sem_signal(&cannibal);
    }
    else if (numMiss >= 2) {
        numMiss -= 2;
        sem_signal(&mutex);
        sem_signal(&missionary);
        sem_signal(&missionary);
    }
    else {
        numCan++;
        sem_signal(&mutex);
        sem_acquire(&cannibal);
        sem_acquire(&row);
        sem_acquire(&mutex);
        printf(1, "before: %d", num_threads);
        num_threads--;
        printf(1, "  after: %d\n", num_threads);
        sem_signal(&mutex);
        texit();
    }
    RowBoat();
    sem_signal(&row);
    sem_signal(&row);
    sem_acquire(&mutex);
    printf(1, "before: %d", num_threads);
    num_threads--;
    printf(1, "  after: %d\n", num_threads);
    sem_signal(&mutex);
    texit();
}


void MissionaryArrives(void *arg) {
    sem_acquire(&mutex);
    if (numMiss >= 2) {
        numMiss -= 2;
        sem_signal(&mutex);
        sem_signal(&missionary);
        sem_signal(&missionary);
    }
    else if (numMiss >= 1 && numCan >= 1) {
        numMiss--;;
        numCan--;
        sem_signal(&mutex);
        sem_signal(&missionary);
        sem_signal(&cannibal);
    }
    else {
        numMiss++;
        sem_signal(&mutex);
        sem_acquire(&missionary);
        sem_acquire(&row);
        sem_acquire(&mutex);
        printf(1, "before: %d", num_threads);
        num_threads--;
        printf(1, "  after: %d\n", num_threads);
        sem_signal(&mutex);
        texit();
    }
    RowBoat();
    sem_signal(&row);
    sem_signal(&row);
    sem_acquire(&mutex);
    printf(1, "before: %d", num_threads);
    num_threads--;
    printf(1, "  after: %d\n", num_threads);
    sem_signal(&mutex);
    texit();
}

