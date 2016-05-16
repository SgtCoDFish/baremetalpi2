/**
 * hex.c
 * A super simple bootloader program with extra debugging support
 * designed to help the machine-code bootstrapping process on a
 * ARM Cortex A7 based Raspberry Pi 2.
 */

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include <stdint.h>

#define PERIPHERAL_BASE 0x3F000000

#define AUX_ENABLE PERIPHERAL_BASE + 0x00215004

#define UART_IO   PERIPHERAL_BASE + 0x00215040
#define UART_IER  PERIPHERAL_BASE + 0x00215044
#define UART_IIR  PERIPHERAL_BASE + 0x00215048
#define UART_LCR  PERIPHERAL_BASE + 0x0021504C
#define UART_LSR  PERIPHERAL_BASE + 0x00215054
#define UART_CNTL PERIPHERAL_BASE + 0x00215060
#define UART_BAUD PERIPHERAL_BASE + 0x00215068

#define GPIO_SEL1      PERIPHERAL_BASE + 0x00200004
#define GPIO_UD        PERIPHERAL_BASE + 0x00200094
#define GPIO_UD_CLOCK0 PERIPHERAL_BASE + 0x00200098

static int strlen(const char *s);
static void uart_init();
static char uart_get();
static void uart_put(char val);
static void uart_puts(const char *str);
static void uart_puthex(uint32_t val);
extern void dummy();
void put_mem(uint32_t address, uint32_t value);
uint32_t get_mem(uint32_t address);

void put_mem(uint32_t address, uint32_t value) {
	*((volatile uint32_t *) address) = value;
}

uint32_t get_mem(uint32_t address) {
	return *((volatile uint32_t *) address);
}

static void uart_put(char val) {
	while((get_mem(UART_LSR) & 0x20) == 0) {}
	
	put_mem(UART_IO, val);
}

static void uart_puts(const char *str) {
	const int len = strlen(str);
	
	for(int i = 0; i < len; ++i) {
		uart_put(str[i]);
	}
}

static void uart_puthex(uint32_t value) {
	// taken from dwelch/rpi2
	uint32_t shift;
	uint32_t tempchar;

	shift = 32;
	while(1) {
		shift -= 4;
		tempchar = (value >> shift) & 0xF;
		
		if(tempchar>9) {
			tempchar+=0x37;
		} else {
			tempchar+=0x30;
		}
		
		uart_put(tempchar);
		
		if(shift == 0) {
			break;
		}
	}
}

static char uart_get() {
	while((get_mem(UART_LSR) & 0x1) == 0) {}
	
	return get_mem(UART_IO);
}

static void uart_init() {
	unsigned int tmp;
	
	put_mem(AUX_ENABLE, 1);
	put_mem(UART_IER, 0);
	put_mem(UART_CNTL, 0);
	put_mem(UART_LCR, 3);
	put_mem(UART_IER, 0);
	put_mem(UART_IIR, 0xC6);
	put_mem(UART_BAUD, 270);
	
	tmp = get_mem(GPIO_SEL1);
	
	tmp &= ~(7 << 12);
	tmp |= 2 << 12;
	
	tmp &= ~(7 << 15);
	tmp |= 2 << 15;
	
	put_mem(GPIO_SEL1, tmp);
	put_mem(GPIO_UD, 0);
	
	for(int i = 0; i < 150; ++i) {
		dummy();
	}
	
	put_mem(GPIO_UD_CLOCK0, ((1 << 14) | (1 << 15)));
	
	for(int i = 0; i < 150; ++i) {
		dummy();
	}
	
	put_mem(GPIO_UD_CLOCK0, 0);
	put_mem(UART_CNTL, 3);
}

static int strlen(const char *s) {
	int len = 0;
	
	while(*s != '\0') {
		len++;
		s++;
	}
	
	return len;
}

#define MEM_START 0x4000

int kernel_main() {
	uart_init();
	uart_puts("debugc\r\n");

	// wait for 0x02 to start listening for input
	while(true) {
		const char c = uart_get();
		if (c == 0x02) {
			break;
		}
	}
	
	bool comment_mode = false;
	uint32_t temp_num = 0x00000000;
	uint8_t num_shift = 28;
	uint32_t mem_pos = MEM_START;
	uint32_t vals_read = 0;

	while(true) {
		char c = uart_get();
		
		if(c == 0x03) {
			break;
		}
		
		if(c == 0x23) {
			comment_mode = true;
		}
		
		if(comment_mode) {
			if(c == 0x10) { // '\n'
				comment_mode = false;
			}
			
			continue;
		}
		
		if(c < 0x30) { // ignore whitespace
			continue;
		}
		
		if(c <= 0x39) {
			c -= 0x30;
		} else {
			c -= 0x57;
		}
		
		temp_num += (((uint32_t)c) << num_shift);
		
		if(num_shift == 0) {
			num_shift = 28;
			uart_puthex(temp_num);
			put_mem(mem_pos, temp_num);
			mem_pos += sizeof(uint32_t);
			temp_num = 0x00000000;
			++vals_read;
		} else {
			num_shift -= 4;
		}
	}
	
	for(int i = 0; i < vals_read; ++i) {
		uart_puthex(get_mem(MEM_START + (i * 4)));
	}
	
	// uart_puts("\r\nDONE\r\n");
	
	while(true) {
		dummy();
	}
}
