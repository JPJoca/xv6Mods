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

	if(argint(0, &n) < 0)
		return -1;
	addr = myproc()->sz;
	if(growproc(n) < 0)
		return -1;
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
sys_share_data(void){

	char* ime;
	void* addr;
	int velicina;
	struct proc* proc = myproc();

	if(argstr(0, &ime) < 0)
		return -1;
	if(argint(2, &velicina) < 0)
		return -1;
	if(argptr(1, &addr, velicina) < 0)
		return -1;
	
	int i = 0;
	for(i;i<10 && proc->list[i].velicina!=0;i++)
		if(strncmp(proc->list[i].naziv, ime, 10) == 0)
			return -2;
		
	if(i==10)
		return -3;
	
		proc->list[i].shared = addr;
		strncpy(proc->list[i].naziv, ime, 10);
		proc->list[i].velicina = velicina;
		return i;

}
int
sys_get_data(void){
	
	char * ime;
	void ** adresa;
	struct proc* proc = myproc();

	if(argstr(0, &ime) < 0)
		return -1;
	if(argptr(1, (void*)&adresa,sizeof(adresa)) < 0)
		return -1;

	int i = 0;
	int flag = 0;
	while(i<SSIZE){
		
		if(strncmp(ime,proc->list[i].naziv, 10) == 0){
			*adresa = proc->list[i].shared;
			flag = 1;
			break; 
		}
		i++;
	}

	if(flag)
		return -2;
	return 0;
}