#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "semaphore.h"

void sem_init(Semaphore *s, int i) {
  int j;
  s->count = i;
  initlock(&s->lock, "s");
  for(j = 0; j < NPROC; j++) {
    s->thread[j] = 0;
  }
  s->next = s->tail = 0;
}

void sem_acquire(Semaphore *s) {
  acquire(&s->lock);
  while(s->count == 0) {
    s->thread[s->tail] = proc;
    s->tail = (s->tail + 1) % NPROC;
    sleep(proc, &s->lock);
  }
  s->count = s->count - 1;
  release(&s->lock);
}

void sem_signal(Semaphore *s) {
  acquire(&s->lock);
  s->count = s->count + 1;
  if(s->thread[s->next]) {
    wakeup(s->thread[s->next]);
    s->thread[s->next] = 0;
    s->next = (s->next + 1) % NPROC;
  }
  release(&s->lock);
}
