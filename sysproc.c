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
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;
  struct proc* curproc = myproc();

  if(argint(0, &n) < 0)
    return -1;
  addr = curproc->sz;
  if (n < 0 && growproc(n) < 0)
    return -1;

  // Avoid heap grows higher than stack.
  if (curproc->sz + n > USERTOP - curproc->stack_size - PGSIZE)
    return -1;

  curproc->sz += n;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
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
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


int sys_nfpgs(void)
{
  return get_num_free_pages();
}

// return 1 when succeed, return 0 when fail
int 
sys_createshm(void)
{
  int sig, bytes;
  if(argint(0, &sig) < 0 || argint(1, &bytes) < 0)
  {
    return 0;
  }
  return createshm(sig,bytes);
}

// return 1 when succeed, return 0 when fail.
int
sys_deleteshm(void)
{
  int sig;
  if(argint(0, &sig) < 0)
  {
    return 0;
  }
  return deleteshm(sig);
}

//return the number of characters actually written in
int
sys_writeshm(void)
{
  int sig;
  char* str;
  int num;
  int offset;
  if(argint(0, &sig) < 0 || argstr(1, &str) < 0 || argint(2, &num) < 0 || argint(3, &offset) < 0)
  {
    return 0;
  }
  return writeshm(sig, str, num, offset);
}

// return the number of characters actually read from shmpages
int 
sys_readshm(void)
{
  int sig;
  char* str;
  int num;
  int offset;
  if(argint(0, &sig) < 0 || argstr(1, &str) < 0 || argint(2, &num) < 0 || argint(3 ,&offset) < 0)
  {
    return 0;
  }
  return readshm(sig, str, num, offset);
}
