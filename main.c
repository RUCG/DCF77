#include <stdlib.h>
#include <avr/io.h>
#include "lcd.h"   //" = Lib in akt Projektverzeichnis
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 8000000L
#endif
#include <util/delay.h>



short x = 0;
uint8_t y;
uint8_t z;

		
int main(void)
{

lcd_init(LCD_DISP_ON_CURSOR_BLINK);

	uint8_t sig;
	uint8_t sig_alt;
	
while(1)
{
	sig = PINC & (1<<PC1);
	
	if (sig != sig_alt) 
	{
		sig_alt = sig;
		
		if(sig != 0)
		{
			lcd_gotoxy(0,0);
			lcd_count_16(x);
			x++;
		}
		
	}
}
return 0;
}