 
 #include "types.h"
 #include "user.h"
void test2(){
	printf(1,"hi" );
}

 int
 main(int argc, char * argv[]) {
   int pid;
   int status;

   if (!(pid = fork())) {
     goto testt;
     exit(0x9f);
   } else {
     wait( & status);
   }
   if (status == 0x9) {
     printf(1, "TEST1:OK\n");
   } else {
     printf(1, "FAILED\n");
   }
   testt:
     return 9;

 }
