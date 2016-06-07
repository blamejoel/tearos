#include "types.h"
#include "user.h"
#include "semaphore.h"

#define TEST_CASE 21

int count_cann, count_mission, thread_num;
Semaphore mutex, cannibal, missionary, row;

void row_boat() {
    printf(1, "Boat is ready, rowing!\n");
}

void arrival_cannibal(void *arg) {
    sem_acquire(&mutex);
    if(count_cann >= 2) {
        count_cann -= 2;
        sem_signal(&mutex);
        sem_signal(&cannibal);
        sem_signal(&cannibal);
    }
    else if(count_mission >= 2) {
        count_mission -= 2;
        sem_signal(&mutex);
        sem_signal(&missionary);
        sem_signal(&missionary);
    }
    else {
        count_cann++;
        sem_signal(&mutex);
        sem_acquire(&cannibal);
        sem_acquire(&row);
        sem_acquire(&mutex);
        printf(1, "before: %d, ", thread_num);
        thread_num--;
        printf(1, "after: %d\n", thread_num);
        sem_signal(&mutex);
        texit();
    }
    row_boat();
    sem_signal(&row);
    sem_signal(&row);
    sem_acquire(&mutex);
    printf(1, "before: %d, ", thread_num);
    thread_num--;
    printf(1, "after: %d\n", thread_num);
    sem_signal(&mutex);
    texit();
}

void arrival_missionary(void *arg) {
    sem_acquire(&mutex);
    if(count_mission >= 2) {
        count_mission -= 2;
        sem_signal(&mutex);
        sem_signal(&missionary);
        sem_signal(&missionary);
    }
    else if(count_mission >= 1 && count_cann >= 1) {
        count_mission--;;
        count_cann--;
        sem_signal(&mutex);
        sem_signal(&missionary);
        sem_signal(&cannibal);
    }
    else {
        count_mission++;
        sem_signal(&mutex);
        sem_acquire(&missionary);
        sem_acquire(&row);
        sem_acquire(&mutex);
        printf(1, "before: %d, ", thread_num);
        thread_num--;
        printf(1, "after: %d\n", thread_num);
        sem_signal(&mutex);
        texit();
    }
    row_boat();
    sem_signal(&row);
    sem_signal(&row);
    sem_acquire(&mutex);
    printf(1, "before: %d, ", thread_num);
    thread_num--;
    printf(1, "after: %d\n", thread_num);
    sem_signal(&mutex);
    texit();
}

int main() {

    count_cann = count_mission = thread_num = 0;

    thread_num = 0;
    sem_init(&mutex, 1);
    sem_init(&cannibal, 0);
    sem_init(&missionary, 0);
    sem_init(&row, 0);

    int i;
    int *tid;

    printf(1, "Testing with %d randomly generated cannibals and missionaries\n", 
        TEST_CASE);
    for(i = 1; i < TEST_CASE+1; i++) {
        if((random(99999*i)/101)%2) {
            tid = thread_create(arrival_cannibal, (void*)0);
            if(tid == 0) {
                printf(1, "something went wrong with thread creation!\n");
                exit();
            }
            sem_acquire(&mutex);
            thread_num++;
            sem_signal(&mutex);
        }
        else {
            tid = thread_create(arrival_missionary, (void*)0);
            if(tid == 0) {
              printf(1, "something went wrong with thread creation!\n");
            }
            sem_acquire(&mutex);
            thread_num++;
            sem_signal(&mutex);
        }
    }
    for(i = 0; i < 3; i++){
        tid = thread_create(arrival_missionary, (void*)0);
        if(tid == 0) {
          printf(1, "something went wrong with thread creation!\n");
        }
        sem_acquire(&mutex);
        thread_num++;
        sem_signal(&mutex);
    }

    printf(1, "done!");
    while(wait()>0);
    exit();

    return 0;
}
