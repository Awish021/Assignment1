#include "types.h"
#include "stat.h"
#include "user.h"

int global = 0;


void func1(int x){
  printf(1,"handler 1 got %d\n",x);
  global++;
}

void func2(int x){
  printf(1,"handler 2 got %d\n",x);
}
void func7(int x){
  printf(1,"handler 7 got %d\n",x);
  exit(0); 
}

void func0(int x){
  printf(1,"handler 0 got %d\n",x);
  global = 1;
}
void handle(int num){
  printf(1,"proc %d recieved %d\n",getpid(),num);
  
}

int main (int argc, char *argv[]){
/* int i;
 
 

 for(i=0; i<32;i++)
  signal(i, handle);
 for(i=0; i<32; i++)
  sigsend(getpid(), i);

 
 sleep(100);
 return 0;
}*/


  int pid = fork();
  if(!pid){
    signal(1, handle);
    signal(4, handle);
    
    sigsend(getpid(),1);
   // sleep(100);
  
  //  sigsend(getpid(),5);
  //  sleep(100);
  }
  else
    wait(0);
 
 return 0; 
} 


/*
int main(int argc, char** argv){  
  int i;
  for(i=0 ; i< 10 ; i++)
    signal(i,handle);
  for(i=0 ; i < 10 ; i++)
    sigsend(getpid(),i);
  
  sleep(10);
  return 0;
}*/
  /*
  signal(0,func0);
  int parent_pid = getpid();
  int child_pid = fork();
  
  if(!child_pid){
    signal(1,func1);
    sigsend(parent_pid, 0);
    while(global==0){}
    printf(1,"Got signal from parent\n");
  }
  else{
    while(global==0){
    }
    printf(1,"Got signal from child\n");
    sigsend(child_pid,1);
    wait(0);
  }


  return 0;
}*/
/*
 int pid = fork();
  if (!pid){
    signal(0,func0);
    signal(1,func1);
    signal(2,func2);
    signal(7,func7);
    
  
    sigsend(getpid(),0);
    sigsend(getpid(),1);
    sigsend(getpid(),2);
    sigsend(getpid(),4);
  //  sigsend(getpid(),7);
    
    while(1);
  

  }
  else{
    wait(0);
    printf(1,"CHILD FINISH!!!\n");
  }

return 0;
}*/
