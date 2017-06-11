ARMPREFIX ?= arm-none-eabi

CARGS = -std=c11 -Wall -O2 -nostdlib -nostartfiles -ffreestanding

all: kernel.img

clean:
	rm -f *.o
	rm -f *.img
	rm -f *.elf

start.o:
	$(ARMPREFIX)-as -c start.s -o start.o

asm.o:
	$(ARMPREFIX)-gcc $(CARGS) -c asm.c -o asm.o

kernel.img: linkfile start.o asm.o
	$(ARMPREFIX)-ld start.o asm.o -T linkfile -o out.elf
	$(ARMPREFIX)-objcopy out.elf -O binary kernel.img
