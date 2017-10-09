#include <avr/io.h>

void uart_init(void)
{
	uint16_t ubrr_val = 25;	/* for 38400 bps at 16MHz */
	UBRRH = (ubrr_val >> 8) & 0xFF;
	UBRRL = (ubrr_val >> 0) & 0xFF;
	UCSRB |= (1 << TXEN);
}

void uart_putc(const char c)
{
	while (!(UCSRA & (1 << UDRE)))
		;
	UDR = c;
}

void uart_puts(const char *s)
{
	while (*s != '\0')
		uart_putc(*s++);
}
