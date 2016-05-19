.globl _start
_start:
	mov sp, #0x08000000
	bl kernel_main

.globl asm_branch
asm_branch:
	bx r0
	bx lr

.globl dummy
dummy:
	bx lr
