 
 #include "types.h"
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

/*#include "types.h"
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
}*/

void handler(int num){
  y++;
  printf(1,"Process %d recieved signal %d\n",getpid(),num);
}

void otherHandler(int num){
  exit(0);
  x++;
  printf(1,"Process %d recieved signal %d\n",getpid(),num);
  exit(0);
}