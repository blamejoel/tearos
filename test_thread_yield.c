#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"
#include "semaphore.h"

void test_thread_yield(void* arg);

int main(void) {
  void* tid;
  int i;
  for(i = 0; i < 4; i++) {
    tid = thread_create(test_thread_yield, (void*)&i);
    if(tid == 0) {
      printf(1, "thread_create %d failed \n",tid);
    }
  }

  while(wait()>= 0)
  printf(1,"All threads yielded properly, woop.\n");
  exit();

  return 0;
}


void test_thread_yield(void *arg){
  /* int thread = *(int*)arg; */
  printf(1, "Yielding thread\n");

  thread_yield();

  printf(1, "Exiting thread\n");
  texit();
}
