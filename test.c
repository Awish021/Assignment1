 #include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
int pid;
int status;

if (!(pid = fork()))
{
	goto testt; /////got bug here.. need to push another eax at assembly and then try again.
exit(0x9f);
}
else
{
wait(&status);
}
if (status == 0x9)
{
printf(1, "OK\n");
}
else
{
printf(1, "FAILED\n");
}
testt:
	return 9;
}
