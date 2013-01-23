#ifndef LCD_H_
#define LCD_H_

#include <msp430.h>

void lcd_wait(volatile unsigned int t);
void lcd_init(void);
void lcd_data(unsigned char l);
void lcd_print(char *string);
void lcd_cursor_xy(unsigned char x, unsigned char y);

#endif /* LCD_H_ */
