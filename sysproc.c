#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  proc->count++;                    // increment syscall counter
  return fork();
}

int
sys_exit(void)
{
  int status;                       // status for syscall exit (lab1)
  argint(0, &status);               // first parameter for syscall (lab1)
  proc->count++;                    // increment syscall counter
  exit(status);                     // add status as an exit param (lab1)
  return 0;  // not reached
}

int
sys_wait(void)
{
  proc->count++;                    // increment syscall counter
  int *status;
  argptr(0, (char**) &status, sizeof(int*));
  return wait(status);
}

int
sys_kill(void)
{
  proc->count++;                    // increment syscall counter
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  proc->count++;                    // increment syscall counter
  return proc->pid;
}

int
sys_sbrk(void)
{
  proc->count++;                    // increment syscall counter
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  proc->count++;                    // increment syscall counter
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  proc->count++;                    // increment syscall counter
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// return number of system calls
int sys_count(void) {
  proc->count++;                    // increment syscall counter
  return proc->count;
}
