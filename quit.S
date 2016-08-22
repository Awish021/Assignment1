# Initial process execs /init.

#include "syscall.h"
#include "traps.h"


# exec(init, argv)
.globl quit


quit:
  movl %eax, %ebx	
  movl $SYS_exit, %eax
  push %ebx
  int $T_SYSCALL