
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{	if(argc<2){
	printf(1, "usage: not enough arguments...\n");
    exit(0);
	}
	int p =atoi(argv[1]);
	if(p>=1&&p<=3){
		schedp(p);
		exit(0);
	}
	else
		printf(1,"Wrong policy");
	exit(0);

}
