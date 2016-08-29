

#include "syscall.h"
#include "traps.h"
#include "asm.h"

.globl sig
.globl sig_label_start

sig:
	sig_label_start:
	  pushl %eax
	  movl $SYS_sigreturn, %eax
	  int $T_SYSCALL
	  ret
	 .globl sig_label_end

	 sig_label_end:
		