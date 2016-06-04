#include "types.h"
#include "user.h"
#include "semaphore.h"

#define HNUM 12
#define ONUM 6

Semaphore H, isReady, O;

void hReady(void * arg_ptr);
void oReady(void *water_molecules);

int main(int argc, char *argv[]) {
  int h2o = 0;
  int arg = 0;

  printf(1, "Water molecules started with: %d\n", h2o);

  sem_init(&H, 0);
  sem_init(&isReady, 1);
  sem_init(&O, 0);

  int i;
  void *tid;
  for(i = 0; i < HNUM; i++) {
     tid = thread_create(hReady, (void *)&arg);
     if(tid <= 0){
         printf(1,"something went wrong when making the %dth H thread!", i);
         exit();
     } 
  }
  for(i = 0; i < ONUM; i++) {
     tid = thread_create(oReady, (void *)&h2o);
     if(tid <= 0){
         printf(1,"something went wrong when making the %dth O thread!", i);
         exit();
     } 
  }
  while(wait()>= 0)
  printf(1, "Water molecules produced: %d\n", h2o);

  exit();
}

void hReady(void *arg_ptr) {
  sem_signal(&H);
  sem_acquire(&O);
  texit();
}

void oReady(void *water_molecules) {
  int *water = (int*)water_molecules;
  sem_acquire(&H);
  sem_acquire(&H);
  sem_signal(&O);
  sem_signal(&O);
  sem_acquire(&isReady);
  *water = *water + 1;
  sem_signal(&isReady);
  texit();
}
