#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include "param.h"
#include "spinlock.h"

typedef struct {
    int count;
    struct spinlock lock;
    void *thread[NPROC];
    int next, tail;
} Semaphore;
#endif
