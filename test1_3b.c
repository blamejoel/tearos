#include "types.h"
#include "user.h"
#include "semaphore.h"

void MonkeyThread(void *arg);
void DominantThread(void *arg);

void monkeyClimbsUp() { printf(1, "Monkey climbing up!\n"); }
void dominantClimbsUp() { printf(1, "Dominant Monkey climbing up!\n"); }

void monkeyClimbsDown() { printf(1, "Monkey climbing down!\n"); }
void dominantClimbsDown() { printf(1, "Dominant Monkey climbing down!\n"); }

void monkeyGrabsCoconut() {printf(1,"Monkey grabs coconut!\n");}
void dominantGrabsCoconut() {printf(1, "Dominant monkey grabs coconut!\n");}

void randomTest();
void edgeTest();
void simpleTest();

Semaphore monkey, one_monkey, dominant_monkey, tree, mutex1, mutex2;

int monkey_cnt, dominant_cnt;
int total_climbed, monkeys, dominants;

int main()  {
  printf(1,"Testing random case\n");
  randomTest();
  printf(1, "\n\n\nTesting simple case\n");
  simpleTest();
  printf(1, "\n\n\nTesting edge case\n");
  edgeTest();
  exit();
}

void simpleTest() {
  total_climbed = 0;
  monkeys = 0;
  dominants = 0;
  monkey_cnt = 0;
  dominant_cnt = 0;

  sem_init(&one_monkey, 1);
  sem_init(&monkey, 1);
  sem_init(&dominant_monkey, 1);
  sem_init(&tree, 3);
  sem_init(&mutex1, 1);
  sem_init(&mutex2, 1);

  int *tid;
  tid = thread_create(MonkeyThread, (void*)0);
  monkey_cnt++;
  if(tid == 0){
    printf(1,  "creating thread failed\n");
    exit();
  }
  tid = thread_create(DominantThread, (void*)0);
  dominant_cnt++;
  if(tid == 0) {
    printf(1,  "creating thread failed\n");
    exit();
  }
  tid = thread_create(MonkeyThread, (void*)0);
  monkey_cnt++;
  if(tid == 0){
    printf(1,  "creating thread failed\n");
    exit();
  }
  printf(1,"Testing %d monkeys and %d dominant monkeys\n", monkey_cnt, dominant_cnt);
  while(wait()>0);
}

void randomTest() {
  total_climbed = 0;
  monkeys = 0;
  dominants = 0;
  monkey_cnt = 0;
  dominant_cnt = 0;

  sem_init(&one_monkey, 1);
  sem_init(&monkey, 1);
  sem_init(&dominant_monkey, 1);
  sem_init(&tree, 3);
  sem_init(&mutex1, 1);
  sem_init(&mutex2, 1);

  int *tid;
  int i = 0;
  for (; i < 20; i++) {
    if ((random(99999*(i+1))/101)%10) {
      tid = thread_create(MonkeyThread, (void*)0);
      monkey_cnt++;
      if(tid == 0){
        printf(1,  "creating thread failed\n");
        exit();
      }
    }
    else {
      tid = thread_create(DominantThread, (void*)0);
      dominant_cnt++;
      if(tid == 0) {
        printf(1,  "creating thread failed\n");
        exit();
      }
    }
  }
  printf(1,"Testing %d monkeys and %d dominant monkeys\n", monkey_cnt, dominant_cnt);
  while(wait()>0);
}

void edgeTest() {
  total_climbed = 0;
  monkeys = 0;
  dominants = 0;
  monkey_cnt = 0;
  dominant_cnt = 0;

  sem_init(&one_monkey, 1);
  sem_init(&monkey, 1);
  sem_init(&dominant_monkey, 1);
  sem_init(&tree, 3);
  sem_init(&mutex1, 1);
  sem_init(&mutex2, 1);

  int *tid;
  tid = thread_create(MonkeyThread, (void*)0);
  monkey_cnt++;
  if(tid == 0){
    printf(1,  "creating thread failed\n");
    exit();
  }
  tid = thread_create(DominantThread, (void*)0);
  dominant_cnt++;
  if(tid == 0) {
    printf(1,  "creating thread failed\n");
    exit();
  }
  tid = thread_create(MonkeyThread, (void*)0);
  monkey_cnt++;
  if(tid == 0){
    printf(1,  "creating thread failed\n");
    exit();
  }
    tid = thread_create(MonkeyThread, (void*)0);
  monkey_cnt++;
  if(tid == 0){
    printf(1,  "creating thread failed\n");
    exit();
  }
  tid = thread_create(DominantThread, (void*)0);
  dominant_cnt++;
  if(tid == 0) {
    printf(1,  "creating thread failed\n");
    exit();
  }
  tid = thread_create(MonkeyThread, (void*)0);
  monkey_cnt++;
  if(tid == 0){
    printf(1,  "creating thread failed\n");
    exit();
  }
    tid = thread_create(MonkeyThread, (void*)0);
  monkey_cnt++;
  if(tid == 0){
    printf(1,  "creating thread failed\n");
    exit();
  }
  tid = thread_create(DominantThread, (void*)0);
  dominant_cnt++;
  if(tid == 0) {
    printf(1,  "creating thread failed\n");
    exit();
  }
  tid = thread_create(MonkeyThread, (void*)0);
  monkey_cnt++;
  if(tid == 0){
    printf(1,  "creating thread failed\n");
    exit();
  }
  printf(1,"Testing %d monkeys and %d dominant monkeys\n", monkey_cnt, dominant_cnt);
  while(wait()>0);
}

// Inspired by the readers and writers with priority solution on piazza
void MonkeyThread(void *arg){
  printf(1,"Monkey arrives!\n");
  sem_acquire(&one_monkey);
  sem_acquire(&monkey);
  sem_acquire(&mutex1);
  monkeys++;
  if (monkeys == 1) {
    sem_acquire(&dominant_monkey);
  }
  sem_signal(&mutex1);
  sem_signal(&monkey);
  sem_signal(&one_monkey);
  sem_acquire(&tree);
  monkeyGrabsCoconut();
  sem_signal(&tree);
  sem_acquire(&mutex1);
  monkeys--;
  total_climbed++;
  if (monkeys == 0) {
    sem_signal(&dominant_monkey);
  }
  sem_signal(&mutex1);
  texit();
}

// Inspired by the readers and writers with priority solution on piazza
void DominantThread(void *arg){
  printf(1,"Dominant monkey arrives!\n");
  sem_acquire(&mutex2);
  dominants++;
  if (dominants == 1) {
    sem_acquire(&monkey);
  }
  sem_signal(&mutex2);
  sem_acquire(&dominant_monkey);
  sem_acquire(&tree);
  dominantGrabsCoconut();
  sem_signal(&tree);
  sem_signal(&dominant_monkey);
  sem_acquire(&mutex2);
  total_climbed++;
  dominants--;
  if (dominants == 0) {
    sem_signal(&monkey);
  }
  sem_signal(&mutex2);
  texit();
}
