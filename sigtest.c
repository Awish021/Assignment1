#include "types.h"
#include "stat.h"
#include "user.h"

int
sig_handler_1(int signum){
  printf(1, "this is signal handler #1. signum = %d\n", signum);
  return 1;
}

int
sig_handler_2(int signum){
  printf(1, "this is signal handler #2. signum = %d\n", signum);
  return 2;
}

int
sig_handler_3(int signum){
  printf(1, "this is signal handler #3. signum = %d\n", signum);
  return 3;
}

int
main(void)
{
  printf(1, "signal test - start\n");
  int parentpid = getpid();
  int i, pid;
  for(i=0; i<1; i++){
    pid = fork();
    if(pid == 0){
      //signal(1,(sighandler_t)(&sig_handler_1));
              printf(1, "#####################################\n");
      while(1){
      sigsend(parentpid,2);    

      }
    }
    else{
              printf(1, "*************************************\n");

      while(1){
        sigsend(pid,1);
      }
      wait(0);
    }
  }  
  exit(0);
} 
 /*#include "types.h"
 #include "user.h"

  int x=0;
int y=0;
void handler(int num);
void otherHandler(int num);
  int
 main(int argc, char * argv[]) {
 printf(1,"Handler: %p\n", handler);
  printf(1,"otherHandler: %p\n", otherHandler);
  int pid = fork();
  
  if(pid!=0){
    
    sigsend(pid,1);
    return 0;
  }
  signal(1,otherHandler);
 while(1);
  return 3;
 }

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void handler(int num);
void otherHandler(int num);
void delay();
int x=0;
int y=0;

int main(int argc, char *argv[]){

//  int i;
//  i=1;

  printf(1,"Handler: %p\n", handler);
  printf(1,"otherHandler: %p\n", otherHandler);
  int pid = getpid();
  printf(1,"FATHER PID: %d\n", pid);
  signal(4,handler);
  signal(5,otherHandler);
  signal(2,handler);
  signal(3,otherHandler);
  if (fork()==0)
  {
    sigsend(pid, 4);
    sigsend(pid, 5);
    if (fork()==0)
    {
      sigsend(pid, 2);
      sigsend(pid, 3);
      exit(0);
    }
    wait(0);
    exit(0);
  }

  wait(0);
  printf(1,"y = %d\n",y);
  printf(1,"x = %d\n",x);

//  for (i=0;i<30;i++){
//    signal(i,handler);
//    sigsend(getpid(),i);
//  }
//  for (i=17;i<32;i++){
//    signal(i,otherHandler);
//    sigsend(getpid(),i);
//  }

  exit(0);
  return 123;
}

void handler(int num){
  y++;
  printf(1,"Process %d recieved signal %d\n",getpid(),num);
}

void otherHandler(int num){
  exit(0);
  x++;
  printf(1,"Process %d recieved signal %d\n",getpid(),num);
  exit(0);
}*/