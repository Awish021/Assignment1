
#include "types.h"
#include "stat.h"
#include "user.h"
#include "perf.h"

struct perf p;

void timeConsuming(){
	priority(1);
	int init = uptime();
	int curr = init;
	int counter =0;
	while(counter<30){
		if(uptime()!=curr){
			curr=uptime();
			counter++;
		}
	}
	exit(0);
}

void blockOnly(){
	int i;
	priority(1000);
	for(i=0;i<30;i++)
		sleep(1);
	exit(0);
}

void mixed(){
	int i;
	priority(50);
	int init = uptime();
	int curr = init;
	int counter =0;
	for(i=0;i<5;i++){
		while(counter<30){
		if(uptime()!=curr){
			curr=uptime();
			counter++;
		}
	}
		sleep(1);
	}
	exit(0);
}

/* struct printing */
void printPerf(struct perf p,int pid){
	printf(2,"the pid is %d\n",pid );
	printf(2,"the turnaround time is %d\n",(p.ttime-p.ctime));
	printf(2,"the ttime is %d\n",p.ttime);
	printf(2,"the stime is %d\n",p.stime);
	printf(2,"the retime is %d\n",p.retime);
	printf(2,"the rutime is %d\n\n",p.rutime);

}

int
main(int argc, char *argv[])
{ 
	printf(1,"Sanity Test ! Calculating results.. please wait..\n\n");
	/*creating 10 proccesses of CPU only*/
	int i,pid;
	int* status=0;
	for(i=0;i<10;i++){
		pid =fork();
		if(pid==0){

			timeConsuming();
		}

	}
	/*creating 10 proccesses of Blocking only*/
	for(i=0;i<10;i++){
		pid =fork();
		if(pid==0){
			
			blockOnly();
		}

	}
	/*creating 10 proccesses of mixed */
	for(i=0;i<10;i++){
		pid =fork();
		if(pid==0)
			
			mixed();
	}
	/* printing results and averages*/
	int wait=0,turnaround=0,running=0,sleep=0;;
	for(i=0;i<30;i++){
		printPerf(p,wait_stat(status,&p));
		wait+=p.retime;
		turnaround+=(p.ttime-p.ctime);
		running+=p.rutime;
		sleep+=p.stime;
	}
	wait=wait/30;
	turnaround=turnaround/30;
	running=running/30;
	sleep=sleep/30;
	printf(1,"The avrages are: \n" );
	printf(2,"waiting time: %d\n",wait);
	printf(2,"turnaround time: %d\n",turnaround);
	printf(2,"running time: %d\n",running);
	printf(2,"sleeping time: %d\n",sleep);

	return 1;
}

