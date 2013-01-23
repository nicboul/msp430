
#include <msp430.h>
#include "lcd.h"

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Watchdog
	P1DIR = 0xFF;
	P1OUT = 0x00;

	lcd_init();

	lcd_cursor_xy(0,1);

	lcd_print("test");

	while(1) {
	}
}
