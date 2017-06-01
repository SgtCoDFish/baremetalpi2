.globl _start

_start:
	ldr r0, .reg
	ldr r1, [r0]

	bic r1, #0x00e00000
	orr r1, #0x00200000

	str r1, [r0]
	sub r0, #0x00000004

	ldr r1, [r0]
	mov r2, #0x00000007
	bic r1, r1, r2, lsl #0x0f
	orr r1, #0x00008000
	str r1, [r0]

	ldr r0, .gpclr1
	mov r2, #0x00000080
	mov r1, r2, lsl #0x08

	ldr r1, [r0]

	mov r1, #0x08
	ldr r1, [r0]

	b hang

hang: b hang

.reg: .word 0x3f200010
.gpclr1: .word 0x3f20002c
