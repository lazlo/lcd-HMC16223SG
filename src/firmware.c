#include "uart.h"
#include "lcd.h"

int main(void)
{
	uart_init();
	uart_puts("UART ready\r\n");

	uart_puts("LCD init ...\r\n");
	lcd_init();
	uart_puts("LCD ready\r\n");

	uart_puts("init done\r\n");
	while (1)
		;
	return 0;
}
