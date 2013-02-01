#include "lcd.h"

void lcd_cmd(unsigned char data)
{
	P1OUT &= ~BIT7;   // because sending command
	P1OUT &= ~BIT6;

	P1OUT &= 0xF0;
	P1OUT |= ((data >> 4) & 0x0F);
	P1OUT |= BIT6;

	lcd_wait(2);

	P1OUT &= ~BIT6;
	P1OUT &= 0xF0;
	P1OUT |= (data & 0x0F);
	P1OUT |= BIT6;

	lcd_wait(2);

	P1OUT  &=~BIT6;
}

void lcd_data(unsigned char l)
{
	P1OUT |= BIT7;    //because sending data
	P1OUT &= ~BIT6;
	P1OUT &= 0xF0;
	P1OUT |=((l >> 4) & 0x0F);

	P1OUT |= BIT6;

	lcd_wait(2);

	P1OUT &= ~BIT6;
	P1OUT &= 0xF0;
	P1OUT |= (l & 0x0F);
	P1OUT |= BIT6;

	lcd_wait(2);

	P1OUT &= ~BIT6;
}

void lcd_init(void)
{
	P1OUT &= ~BIT7;
	P1OUT &= ~BIT6;
	P1OUT |= 0x3;

	lcd_wait(40);

	P1OUT |= BIT6;
	P1OUT &= ~BIT6;

	lcd_wait(5);

	P1OUT |= BIT6;
	P1OUT &= ~BIT6;

	lcd_wait(5);

	P1OUT |= BIT6;
	P1OUT &= ~BIT6;

	lcd_wait(2);

	P1OUT &= 0xF2;
	P1OUT |= BIT6;
	P1OUT &= ~BIT6;

	lcd_cmd(0x28);  // set data length 4 bit 2 line
	lcd_wait(250);

	lcd_cmd(0x0E);  // set display on cursor on blink on
	lcd_wait(250);

	lcd_cmd(0x01);  // clear lcd
	lcd_wait(250);

	lcd_cmd(0x06);  // cursor shift direction
	lcd_wait(250);

	lcd_cmd(0x80);  //set ram address
	lcd_wait(250);
}

void lcd_wait(volatile unsigned int t)
{
	volatile unsigned int i;
	for (t; t > 1; t--) {
		for (i=0 ; i <= 110; i++);
	}
}

void lcd_print(char *string)
{
	while (*string) {
		lcd_data(*string);
		string++;
	}
}

void lcd_cursor_xy(unsigned char x, unsigned char y)
{
	if (x < 40) {
		if (y) {
			x |= 0x40;
		}
		x |= 0x80;
		lcd_cmd(x);
	}
}
/*
void lcd_print_int(int integer)
{

    unsigned char thousands,hundreds,tens,ones;
    thousands = integer / 1000;

    lcd_data(thousands + 0x30);
    
    hundreds = ((integer - thousands*1000)-1) / 100;

	lcd_data(hundreds + 0x30);
	tens = (integer%100)/10;

	lcd_data(tens + 0x30);
	ones = integer%10;

	lcdData( ones + 0x30);
}
*/
