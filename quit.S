

#include "syscall.h"
#include "traps.h"
.text

.globl quit5
.globl quit_label_start

quit5:
	quit_label_start:
	  movl %eax, %ebx
	  movl $SYS_exit, %eax
	  push %ebx
	  push %ebx
	  int $T_SYSCALL
	 .globl quit_label_end

	 quit_label_end:
		ret