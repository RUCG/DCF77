#include <stdlib.h>
#include <avr/io.h>
#include "lcd.h"   //" = Lib in akt Projektverzeichnis
#include <avr/interrupt.h>


#ifndef F_CPU
#define F_CPU 8000000L
#endif


uint16_t pruefer = 1;
uint16_t ISR_zaehler;
uint16_t sec = 1;
uint8_t min = 0;
int8_t min_pruefer = 0;

ISR (TIMER0_OVF_vect)		//Overflow Interrupt Service Routine von Timer0
{		
		TCNT0 = 0;		// Startwert des Zaehlers nach Interrupt.
		ISR_zaehler++;
		if(ISR_zaehler==1)	//entspricht ca 100ms Sekunden (8M / 256 / 256 = 122/10)
		{
			ISR_zaehler=0;
			pruefer++;
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
	//int sec = 1;
	
while(1)
{
	sig = PINC & (1<<PC1);
	
	
	
	if (sig != sig_alt) 			//Flanken triggerung
	{
		sig_alt = sig;
		
		if(sig != 0)			//Positiveflankentriggerung
		{	
			lcd_gotoxy(0,2);
			min_pruefer++;
			lcd_count_16(min_pruefer);
			min_pruefer = 0;
		
			lcd_gotoxy(7,0);
			lcd_count_16(min);			//Minuten ausgabe
			
			lcd_gotoxy(9,0);
			lcd_puts(":");
		
			lcd_gotoxy(10,0);
			lcd_count_16(sec);			//Sekunden ausgabe
			sec++;
			
			pruefer = 0;
		}
		else				//Negativeflankentriggerung
		{
			lcd_gotoxy(1,3);
			lcd_count_16(pruefer);
			pruefer = 0;
			
			
			
			
		}
		
				if(min_pruefer>5000)
				{	
					lcd_gotoxy(10,3);
					lcd_puts("MIN++");
					lcd_gotoxy(7,0);
					lcd_count_16(min);
					sec = 1;
					lcd_clrscr();
					min++;
					
				}
				else
				{
					lcd_gotoxy(10,3);
					lcd_puts("normal");
				}
	}
}
return 0;
}