#include <util/delay.h>

#include "pins.h"

static void lcd_init_pins(void)
{
	/* Set control pins to direction "output" */
	LCD_RS_DDR_REG |= (1 << LCD_RS_PIN);
	LCD_RW_DDR_REG |= (1 << LCD_RW_PIN);
	LCD_EN_DDR_REG |= (1 << LCD_EN_PIN);
	/* Set data pins to direction "output" */
	LCD_D0_DDR_REG |= (1 << LCD_D0_PIN);
	LCD_D1_DDR_REG |= (1 << LCD_D1_PIN);
	LCD_D2_DDR_REG |= (1 << LCD_D2_PIN);
	LCD_D3_DDR_REG |= (1 << LCD_D3_PIN);
	LCD_D4_DDR_REG |= (1 << LCD_D4_PIN);
	LCD_D5_DDR_REG |= (1 << LCD_D5_PIN);
	LCD_D6_DDR_REG |= (1 << LCD_D6_PIN);
	LCD_D7_DDR_REG |= (1 << LCD_D7_PIN);
}

static void lcd_io_write(const uint8_t byte)
{
	if (byte & (1 << 0))	LCD_D0_PORT_REG |= (1 << LCD_D0_PIN);
	else			LCD_D0_PORT_REG &= ~(1 << LCD_D0_PIN);
	if (byte & (1 << 1))	LCD_D1_PORT_REG |= (1 << LCD_D1_PIN);
	else			LCD_D1_PORT_REG &= ~(1 << LCD_D1_PIN);
	if (byte & (1 << 2))	LCD_D2_PORT_REG |= (1 << LCD_D2_PIN);
	else			LCD_D2_PORT_REG &= ~(1 << LCD_D2_PIN);
	if (byte & (1 << 3))	LCD_D3_PORT_REG |= (1 << LCD_D3_PIN);
	else			LCD_D3_PORT_REG &= ~(1 << LCD_D3_PIN);
	if (byte & (1 << 4))	LCD_D4_PORT_REG |= (1 << LCD_D4_PIN);
	else			LCD_D4_PORT_REG &= ~(1 << LCD_D4_PIN);
	if (byte & (1 << 5))	LCD_D5_PORT_REG |= (1 << LCD_D5_PIN);
	else			LCD_D5_PORT_REG &= ~(1 << LCD_D5_PIN);
	if (byte & (1 << 6))	LCD_D6_PORT_REG |= (1 << LCD_D6_PIN);
	else			LCD_D6_PORT_REG &= ~(1 << LCD_D6_PIN);
	if (byte & (1 << 7))	LCD_D7_PORT_REG |= (1 << LCD_D7_PIN);
	else			LCD_D7_PORT_REG &= ~(1 << LCD_D7_PIN);
}

static void lcd_io_mode_set(const uint8_t rs, const uint8_t rw)
{
	if (rs)		LCD_RS_PORT_REG |= (1 << LCD_RS_PIN);
	else		LCD_RS_PORT_REG &= ~(1 << LCD_RS_PIN);
	if (rw)		LCD_RW_PORT_REG |= (1 << LCD_RW_PIN);
	else		LCD_RW_PORT_REG &= ~(1 << LCD_RW_PIN);
}

#define RS_INSTR	0
#define RS_DATA		1

#define RW_WRITE	0
#define RW_READ		1

#define LCD_IO_MODE_WRITE_INSTRUCTION()	lcd_io_mode_set(RS_INSTR, RW_WRITE)	/* 0,0 */
#define LCD_IO_MODE_READ_INSTRUCTION()	lcd_io_mode_set(RS_INSTR, RW_READ)	/* 0,1 */

static int lcd_is_busy(void)
{
	uint8_t busy = 0;
	LCD_IO_MODE_READ_INSTRUCTION();
	LCD_D7_DDR_REG &= ~(1 << LCD_D7_PIN);			/* Set D7 to direction "input" */	
	busy = LCD_D7_PIN_REG & (1 << LCD_D7_PIN) ? 1 : 0;	/* Read Busy Flag */
	LCD_D7_DDR_REG |= (1 << LCD_D7_PIN);			/* Restore D7 direction */
	return busy;
}

static void lcd_funcset(const uint8_t dtlen, const uint8_t lines, const uint8_t font)
{
	uint8_t instr = (1 << 5); /* 0x20 */
	if (dtlen)	instr |= (1 << 4);
	if (lines)	instr |= (1 << 3);
	if (font)	instr |= (1 << 2);
	LCD_IO_MODE_WRITE_INSTRUCTION();
	lcd_io_write(instr);

	LCD_EN_PORT_REG |= (1 << LCD_EN_PIN);
	_delay_us(39);
	LCD_EN_PORT_REG &= ~(1 << LCD_EN_PIN);
}

void lcd_init(void)
{
	lcd_init_pins();
	while (lcd_is_busy())
		;
	/* Set
	 * Interface data length = 8
	 * Display line numbers = 2
	 * Display font type = 5 x 10 dots */
	lcd_funcset(1, 1, 1);
}
