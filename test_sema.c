#include "types.h"
#include "user.h"
#include "semaphore.h"

struct thread{
    int tid;
};

struct{
    lock_t lock;
    struct thread threads[64];
    int total;
}ttable;

Semaphore *sem;

void func(void *arg_ptr);


int main(){
    struct thread *t;
    sem_init(sem, 1);
    int i;
    printf(1, "init ttable\n");
    lock_init(&ttable.lock);
    ttable.total = 0;

    lock_acquire(&ttable.lock);
    for (t=ttable.threads; t<&ttable.threads[64]; t++){
        t->tid = 0;
    }
    lock_release(&ttable.lock);
    printf(1, "testing semaphores \n\n\n");
    for (i = 0; i < 10; i++) {
        thread_create(func, 0);
    }

    i = 0;

    while (&ttable.total > 0){
        printf(1, "total threads: %d", &ttable.total);
    }
    exit();


}

void func(void *arg_ptr) {
    int tid;
    tid = getpid();
    lock_acquire(&ttable.lock);
    (ttable.threads[ttable.total]).tid = tid;
    ttable.total++;
    lock_release(&ttable.lock);

    printf(1, "Thread %d, getting semaphore.", tid);
    sem_acquire(sem);
    printf(1, "I, thread %d, have the semaphore!", tid);
    sem_signal(sem);
    texit();
}
