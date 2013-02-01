
#include <msp430.h>
#include "lcd.h"
#include <msp430.h>
#include <stdio.h> // snprintf()
 
/*
 * Conversion formulas from http://www.43oh.com/forum/viewtopic.php?t=2474
 *
 * ADC to degrees Celsius:
 * C = (A * 27069L - 18169625L) >> 16
 *
 * ADC to degrees Kelvin:
 * K = (A * 27069L - 268467L) >> 16
 *
 * ADC to degrees Farenheit:
 * F = (A * 48724L - 30634388L) >> 16
 *
 * The 'L' character after the integer forces it to be a 32-bit signed long.
 */
 
// it's okay to run this at any MCU clockrate since it uses its own ADC10OSC clock
void chiptemp_setup()
{
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON;
    ADC10CTL1 = INCH_10 + ADC10DIV_3;   // Channel 10 = Temp Sensor
}

void lumen_setup()
{
    ADC10CTL0 = ADC10SHT_2; // select clock speed of 16x
    ADC10CTL0 |= ADC10ON;   // enable ADC10 (10bits, 0-1023)
    ADC10CTL0 |= ADC10IE;   // interrupt enable
    ADC10CTL1 = INCH_4;     // Channel 4
    ADC10AE0 = BIT4;       // P1.4 ADC option
}

int lumen_read()
{
    unsigned adc;

    // ENC = enable conversion, ADC10SC = start conversion
    ADC10CTL0 |= ENC + ADC10SC;
    while (!(ADC10CTL0 & ADC10IFG)); /* wait until conversion is completed */

    adc = ADC10MEM;

    // shut off conversion and lower flag to save power.
    // ADC10SC is reset automatically.
    while (ADC10CTL0 & ADC10BUSY); /* wait for non-busy per section 22.2.6.6 in User's Guide */

    ADC10CTL0 &= ~ENC;
    ADC10CTL0 &= ~ADC10IFG;

    return adc; 
}

int chiptemp_read()
{
    unsigned adc;

    // ENC = enable conversion, ADC10SC = start conversion
    ADC10CTL0 |= ENC + ADC10SC;
    while (!(ADC10CTL0 & ADC10IFG)); /* wait until conversion is completed */

    adc = ADC10MEM;

    // shut off conversion and lower flag to save power.
    // ADC10SC is reset automatically.
    while (ADC10CTL0 & ADC10BUSY); /* wait for non-busy per section 22.2.6.6 in User's Guide */

    ADC10CTL0 &= ~ENC;
    ADC10CTL0 &= ~ADC10IFG;

    // return degrees C
    return (int)((adc * 27069L - 18169625L) >> 16);
}

void chiptemp_readstr(char *str, unsigned len)
{
    snprintf(str, len, "%d%c C", chiptemp_read(), 176);
}

void lumen_readstr(char *str, unsigned len)
{
    snprintf(str, len, "%d", lumen_read());
}

int main(void)
{
    int i = 0;
    char temp[10];

    WDTCTL = WDTPW + WDTHOLD;       // Watchdog
    P1DIR = 0xFF;
    P1OUT = 0x00;

    P1DIR &= ~BIT4; // P1.4 as analog input

    lcd_init();

    while (1) {
        lumen_setup();
        lcd_cursor_xy(0,0);
        lumen_readstr(temp, 10);
        lcd_print(temp);
       
        chiptemp_setup();
        lcd_cursor_xy(0,1); 
        chiptemp_readstr(temp, 10);
        lcd_print(temp);

        for (i=0; i<2; i++)
            __delay_cycles(0xFFFF);
    }
}
