#include <stdlib.h>
#include <avr/io.h>
#include "lcd.h"   //" = Lib in akt Projektverzeichnis
#include <avr/interrupt.h>


#ifndef F_CPU
#define F_CPU 8000000L
#endif

uint16_t pruefer = 1;
uint16_t ISR_zaehler;
int16_t min_pruefer = 1;
uint16_t min = 0;
uint16_t pause = 0;
uint16_t sec = 0;

ISR (TIMER0_OVF_vect)		//Overflow Interrupt Service Routine von Timer0
{		
		TCNT0 = 0;		// Startwert des Zaehlers nach Interrupt.
		ISR_zaehler++;
		if(ISR_zaehler==1)	//entspricht ca 100ms Sekunden (8M / 256 / 256 = 122/10)
		{
			ISR_zaehler=0;
			pruefer++;
			min_pruefer++;
			pause++;
		}
} // end of ISR


		
int main(void)
{

lcd_init(LCD_DISP_ON_CURSOR_BLINK);
lcd_clrscr();

	DDRC = 0x00;
	PORTC = 0x00;
	
	//Konfiguration Timer Overflow
	TCCR0A	= 0x00;			//normal mode
	TCCR0B	= 0x02;			//clk/256
	TIMSK0	|= (1<<TOV0);	//Time0 frei
	TIFR0	|= (1<<TOV0);	//Interrupt Timeroverflow einschalten
	sei();
	
	uint16_t sig;
	uint16_t sig_alt;
	
	
	
	while(1)
	{
		sig = PINC & (1<<PC1);
		
		if (sig != sig_alt) 			//Flanken triggerung
		{
			sig_alt = sig;
			
			
			
			if(sig_alt != 0)			//Positiveflankentriggerung
			{	
				if(pause>6000)
				{	
					sec = 0;
				}
				
				pruefer = 0;
				
				lcd_gotoxy(1,0);
				lcd_puts("    ");
				lcd_gotoxy(1,0);
				lcd_count_16(pause);
				
				lcd_gotoxy(13,0);
				lcd_puts(":");
			
				lcd_gotoxy(14,0);
				lcd_puts("    ");
				lcd_gotoxy(14,0);
				lcd_count_16(sec);			//Sekunden ausgabe
				
			}	
			else			//negative Flanke
			{
				pause = 0;
				
				
				lcd_gotoxy(1,1);
				lcd_puts("   ");
				lcd_gotoxy(1,1);
				lcd_count_16(pruefer);
				
				sec++;
				
				switch(sec)
				{
				
				case 21: min = 0;
						if(pruefer>600)			//Logisches eins
							{
							min = min + 1;
							}
						break;
				case 22: if(pruefer>600)			//Logisches eins
							{
							min = min + 2;
							}
						break;
				case 23: if(pruefer>600)			//Logisches eins
							{
							min = min + 4;
							}
						break;
				case 24: if(pruefer>600)			//Logisches eins
							{
							min = min + 8;
							}
						break;
				case 25: if(pruefer>600)			//Logisches eins
							{
							min = min + 10;
							}
						break;
				case 26: if(pruefer>600)			//Logisches eins
							{
							min = min + 20;
							}
						break;
				case 27: if(pruefer>600)			//Logisches eins
							{
							min = min + 40;
							}
						break;
				}
							lcd_gotoxy(11,0);
							lcd_puts("  ");
							lcd_gotoxy(11,0);
							lcd_count_16(min);			//Minuten ausgabe
			}		
			}
	}
return 0;
}