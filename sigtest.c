#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void handler1(int num);
void handler2(int num);
void handler3(int num);
int x=0;
int y=0;

int main(int argc, char *argv[]){

//  int i;
//  i=1;

  int pid = getpid();
  signal(4,handler1);
  signal(5,handler2);
  signal(2,handler1);
  signal(3,handler2);
  signal(6,handler3);
  if (fork()==0)
  {
    sigsend(pid, 4);
    sigsend(pid, 5);
    sigsend(pid,6);
    if (fork()==0)
    {
      sigsend(pid, 2);
      sigsend(pid, 3);
      sigsend(pid,6);
      exit(0);
    }
    wait(0);
    exit(0);
  }

  wait(0);
  printf(1,"y = %d\n",y);
  printf(1,"x = %d\n",x);

  return 1;
}

void handler1(int num){
  y++;
  printf(1,"Process %d recieved signal %d\n",getpid(),num);
}

void handler2(int num){
  x++;
  printf(1,"Process %d recieved signal %d\n",getpid(),num);
}
void handler3(int num){
  printf(1,"Print only, from proccess %d and signal %d\n",getpid(),num );
}