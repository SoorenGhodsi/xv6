#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "processesinfo.h"
#include "spinlock.h"

extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

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

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;

  myproc()->sz = myproc()->sz + n;
  // if(growproc(n) < 0)
  //   return -1;
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

int
sys_yield(void)
{
  yield();
  return 0;
}

int 
sys_shutdown(void)
{
  shutdown();
  return 0;
}

int
sys_settickets(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  if(n <= 0 || n > 100000)
    return -1;

  // Set the tickets for the current process
  myproc()->tickets = n;
  return 0;
}

int
sys_getprocessesinfo(void)
{
  struct processes_info *proc_info;

  if (argptr(0, (void*) &proc_info, sizeof(*proc_info)) < 0)
    return -1;
  
  acquire(&ptable.lock);

  int count = 0;
  for (struct proc *p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    if (p->state == UNUSED)
      continue;
    
    proc_info->pids[count] = p->pid;
    proc_info->times_scheduled[count] = p->times_scheduled;
    proc_info->tickets[count] = p->tickets;
    count++;
  }
  proc_info -> num_processes = count;

  release(&ptable.lock);

  return 0;
}

// Paging method setup:

int
sys_getpagetableentry(void)
{
  int pid, address;
  if(argint(0, &pid) < 0 || argint(1, &address) < 0)
    return -1;
  return getpagetableentry(pid, address);
}

int
sys_isphysicalpagefree(void)
{
  int ppn;
  if(argint(0, &ppn) < 0)
    return -1;
  return isphysicalpagefree(ppn);
}

int
sys_dumppagetable(void)
{
  int pid;
  if(argint(0, &pid) < 0)
    return -1;
  return dumppagetable(pid);
}
