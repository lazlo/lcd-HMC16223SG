#include "lcd.h"

int main(void)
{
	lcd_init();
	lcd_init();
	lcd_puts("Hello!");
	while (1)
		;
	return 0;
}
