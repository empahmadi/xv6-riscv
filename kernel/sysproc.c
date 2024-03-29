#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_hello(void){
    return hello();
}

int
sys_getProcTick(void){
    int pid;
    argint(0, &pid);
    return getProcTick(pid);
}



int
sys_sysinfo(void) {
    uint64 p;
    argaddr(0, &p);
    return sysinfo(p);
}

int
sys_switch_scheduler(void){
    int number;
    argint(0, &number);
    return switch_scheduler(number);
}

int sys_get_termination_time(void){
    int pid;
    argint(0, &pid);
    return get_termination_time(pid);
}

int sys_get_running_time(void){
    int pid;
    argint(0, &pid);
    return get_running_time(pid);
}

int sys_get_ready_time(void){
    int pid;
    argint(0, &pid);
    return get_ready_time(pid);
}

int sys_get_sleeping_time(void){
    int pid;
    argint(0, &pid);
    return get_sleeping_time(pid);
}

int sys_get_creation_time(void){
    int pid;
    argint(0, &pid);
    return get_creation_time(pid);
}

int sys_wait_pid(void){
    uint64 p;
    uint64 i;
    argaddr(0, &p);
    argaddr(1, &i);
    return wait_pid(p, i);
}

