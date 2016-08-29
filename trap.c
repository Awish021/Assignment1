#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"


// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[];  // in vectors.S: array of 256 entry pointers
extern void update_ticks(void);
struct spinlock tickslock;
uint ticks;

extern void* sig_label_start;
extern void* sig_label_end;

#define IS_SIG_ON(p,signum) (p->pending & (1<<signum))
#define TURN_ON(p,signum) p->pending |= (1<<signum)
#define TURN_OFF(p,signum) p->pending &= (~(1<<signum))


void
tvinit(void)
{
  int i;

  for(i = 0; i < 256; i++)
    SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
  SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);
  
  initlock(&tickslock, "time");
}

void
idtinit(void)
{
  lidt(idt, sizeof(idt));
}

//PAGEBREAK: 41
void
trap(struct trapframe *tf)
{
  if(tf->trapno == T_SYSCALL){
    if(proc->killed)
      exit(0);
    proc->tf = tf;
    syscall();
    if(proc->killed)
      exit(0);
    return;
  }

  switch(tf->trapno){
  case T_IRQ0 + IRQ_TIMER:
    if(cpu->id == 0){
      acquire(&tickslock);
      ticks++;
      update_ticks();
      wakeup(&ticks);
      release(&tickslock);
    }
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE:
    ideintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE+1:
    // Bochs generates spurious IDE1 interrupts.
    break;
  case T_IRQ0 + IRQ_KBD:
    kbdintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_COM1:
    uartintr();
    lapiceoi();
    break;
  case T_IRQ0 + 7:
  case T_IRQ0 + IRQ_SPURIOUS:
    cprintf("cpu%d: spurious interrupt at %x:%x\n",
            cpu->id, tf->cs, tf->eip);
    lapiceoi();
    break;
   
  //PAGEBREAK: 13
  default:
    if(proc == 0 || (tf->cs&3) == 0){
      // In kernel, it must be our mistake.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->trapno, cpu->id, tf->eip, rcr2());
      panic("trap");
    }
    // In user space, assume process misbehaved.
    cprintf("pid %d %s: trap %d err %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            proc->pid, proc->name, tf->trapno, tf->err, cpu->id, tf->eip, 
            rcr2());
    proc->killed = 1;
  }

  // Force process exit if it has been killed and is in user space.
  // (If it is still executing in the kernel, let it keep running 
  // until it gets to the regular system call return.)
  if(proc && proc->killed && (tf->cs&3) == DPL_USER)
    exit(0);

  // Force process to give up CPU on clock tick.
  // If interrupts were on while locks held, would need to check nlock.
  if(proc && proc->state == RUNNING && tf->trapno == T_IRQ0+IRQ_TIMER)
    yield();

  // Check if the process has been killed since we yielded
  if(proc && proc->killed && (tf->cs&3) == DPL_USER)
    exit(0);
}
void update_ctime(struct proc* p){
  p->ctime=ticks;
}

void update_ttime(struct proc* p){
  p->ttime=ticks;
}

int bitXor(int x, int y) 
{
    int a = x & y;
    int b = ~x & ~y;
    int z = ~a & ~b;
    return z;
}
void do_signal(){
  if(proc==0||proc->busy==1)
      return;
  if((proc->tf->cs&3)==3){

    if(proc->pending!=0){
      memmove(&(proc->btf), proc->tf, sizeof(struct trapframe)); /* copy the tf to new backup*/
      int temp = proc->pending,counter;
      for(counter=0;counter<32;counter++){
        int bit =  0x00000001;
        bit=(bit<<counter);
        if((temp&bit)!=0){ /*hit */
         
          proc->pending=bitXor(bit,proc->pending);
         
          int size=(int)(&sig_label_end)-(int)(&sig_label_start);
          proc->busy=1;
          proc->tf->esp-=size;
          uint sp=proc->tf->esp;
          copyout(proc->pgdir,proc->tf->esp,&sig_label_start,size);

          proc->tf->eip=(uint)proc->handlers[counter];

          proc->tf->esp -= 4;
          *((int*) proc->tf->esp) = counter;
          proc->tf->esp -= 4;
          *((int*) proc->tf->esp) = sp;

          break;
        }


      }
    }
  }
}
void handleSignals(){
  if (proc == 0)
    return;
  if (proc->busy == 1)
    return;
  if (!(proc->pending == 0)){
    int i;
    for (i=0; i < NUMSIG; i++){
      if ((proc->pending & (int)(0x01 << i)) != 0){ //CHECKING FOR SIGNAL[i] = 1
        acquire(&ptable.lock);
        memmove(&(proc->btf), proc->tf, sizeof(struct trapframe)); //SAVE TRAPFRAME

        proc->pending &= ~(int)(0x01 << i); //RESET SIGNAL

        int sigreturnSize;
        uint sp;

        proc->busy = 1;
        //PUT SYSTEMCALL SIGRETURN FUNCTION INTO SP
        sigreturnSize = sig_label_end- sig_label_start;
        proc->tf->esp -= sigreturnSize;
        sp = proc->tf->esp;
        copyout(proc->pgdir,proc->tf->esp,sig_label_start,sigreturnSize);

        proc->tf->eip = (uint)(proc->handlers[i]);//PC = SIGNAL HANDLER
        cprintf("this is sp: %x\n",sp);
        cprintf("this is (uint)proc->signalTable[i]: %x\n",(uint)(proc->handlers[i]));
        cprintf("this is sigreturn: %x\n",sigreturn);
        cprintf("this is default: %p\n",default_handler);
        cprintf("this is pointer: %p\n",(uint)(proc->handlers[i]));
        //ARGUMENT + RET TO SYSCALL
        proc->tf->esp -= 4;
        *((int*) proc->tf->esp) = i;
        proc->tf->esp -= 4;
        *((int*) proc->tf->esp) = sp;

        break;
      }
    }
  }
  release(&ptable.lock);
}
/*void handleSignals1(){
  if(proc == 0)  //scheduler
    return;
  if(proc->pending == 0) // no signals
    return;
  int i;
    for(i=0; i<NUMSIG; i++){
      if(IS_SIG_ON(proc,i)){
        //backup the trapframe
      memmove(proc->tfbackup,&tf,sizeof(struct trapframe));
      TURN_OFF(proc,i);
        tf->eip = (uint)(proc->sig_table[i]);
        int length = (int)(&inject_sigreturn_end) - (int)(&sig_label_start);
        tf->esp = (tf->esp - length);
        uint ret_address = tf->esp;
        copyout(proc->pgdir, tf->esp, &sig_label_start, length);       

          tf->esp -= 4;
          *((uint*)tf->esp) = i;
          tf->esp -= 4;
          *((uint*)tf->esp) = ret_address;

break; 
}*/