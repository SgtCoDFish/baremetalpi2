.globl _start
_start:
	mov sp, #0x08000000
	bl kernel_main

.globl dummy
dummy:
	bx lr
