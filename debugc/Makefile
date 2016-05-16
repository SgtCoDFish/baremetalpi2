ARMPREFIX ?= arm-none-eabi

CARGS = -std=c11 -Wall -O2 -nostdlib -nostartfiles -ffreestanding

all: kernel7.img

clean:
	rm -f *.o
	rm -f *.img

start.o:
	$(ARMPREFIX)-as -c start.s -o start.o

hex.o:
	$(ARMPREFIX)-gcc $(CARGS) -c hex.c -o hex.o

kernel7.img: linkfile start.o hex.o
	$(ARMPREFIX)-ld start.o hex.o -T linkfile -o out.elf
	$(ARMPREFIX)-objcopy out.elf -O binary kernel7.img
