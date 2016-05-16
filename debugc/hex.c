/**
 * hex.c
 * A super simple bootloader program with extra debugging support
 * designed to help the machine-code bootstrapping process on a
 * ARM Cortex A7 based Raspberry Pi 2.
 */

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

int kernel_main() {
	uart_init();
	uart_puts("hello, world\r\n");

	while(1) {
		char c = uart_get();
		
		if(c < 0x30) {
			continue;
		} else if(c <= 0x39) {
			uart_put(c);
		} else {
			uart_put('t');
		}
	}
}
