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
uint16_t pause = 0;

uint16_t sec = 0;
uint16_t min = 0;
uint16_t hour = 0;
int16_t weekday = 0;
uint16_t day = 0;
uint16_t month = 0;
uint16_t year = 0;

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
	PORTC = 00110000;
	
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
				if(pause>6000)			//geht in diese if Schlaufe sobald das lange Highsignal detektiert wird.
				{	
					sec = 1;
					
					if(min<10)
					{
					lcd_gotoxy(11,3);
					lcd_puts("  ");
					lcd_gotoxy(12,3);
					lcd_count_16(min);
					}
					else
					{
					lcd_gotoxy(11,3);
					lcd_puts("  ");
					lcd_gotoxy(11,3);
					lcd_count_16(min);
					}
					
					
					
					if(hour<10)
					{
					lcd_gotoxy(9,3);
					lcd_puts("  ");
					lcd_gotoxy(9,3);
					lcd_count_16(hour);	
					}
					else
					{
					lcd_gotoxy(8,3);
					lcd_puts("  ");
					lcd_gotoxy(8,3);
					lcd_count_16(hour);
					}
					
					
					
					if(day<10)
					{
					lcd_gotoxy(7,0);
					lcd_puts("  ");
					lcd_gotoxy(7,0);
					lcd_count_16(day);	
					}
					else
					{
					lcd_gotoxy(6,0);
					lcd_puts("  ");
					lcd_gotoxy(6,0);
					lcd_count_16(day);	
					}
					
					
					
					if(month<10)
					{
						lcd_gotoxy(9,0);
						lcd_puts("  ");
						lcd_gotoxy(10,0);
						lcd_count_16(month);
					}
					else
					{
						lcd_gotoxy(9,0);
						lcd_puts("  ");
						lcd_gotoxy(9,0);
						lcd_count_16(month);
					}	
					
					if((hour==13)&&(min==34))
					{
					while(1)
						{
							PORTC |=(1<<PC4);
							PORTC |=(1<<PC5);
						}
					}
					
					lcd_gotoxy(12,0);
					lcd_puts("20");
					lcd_gotoxy(14,0);
					lcd_puts("  ");
					lcd_gotoxy(14,0);
					lcd_count_16(year);
					
					switch(weekday)
					{
					case 1: lcd_gotoxy(7,1);
							lcd_puts("         ");
							lcd_gotoxy(7,1);
							lcd_puts("MONDAY");
							break;
					case 2: lcd_gotoxy(7,1);
							lcd_puts("         ");
							lcd_gotoxy(7,1);
							lcd_puts("TUESDAY");
							break;
					case 3: lcd_gotoxy(7,1);
							lcd_puts("         ");
							lcd_gotoxy(7,1);
							lcd_puts("WEDNESDAY");
							break;
					case 4: lcd_gotoxy(7,1);
							lcd_puts("         ");
							lcd_gotoxy(7,1);
							lcd_puts("THURSDAY");
							break;
					case 5: lcd_gotoxy(7,1);
							lcd_puts("         ");
							lcd_gotoxy(7,1);
							lcd_puts("FRIDAY");
							break;
					case 6: lcd_gotoxy(7,1);
							lcd_puts("         ");
							lcd_gotoxy(7,1);
							lcd_puts("SATURDAY");
							break;
					case 7: lcd_gotoxy(7,1);
							lcd_puts("         ");
							lcd_gotoxy(7,1);
							lcd_puts("SUNDAY");
							break;
					case 8:	lcd_gotoxy(7,1);
							lcd_puts("         ");
							lcd_gotoxy(7,1);
							lcd_puts("0");
							break;
					}
					
					
				}							//end of if pause
				
				pruefer = 0;
				
				lcd_gotoxy(8,0);
				lcd_puts(".");
				
				lcd_gotoxy(11,0);
				lcd_puts(".");
				
				
				
				lcd_gotoxy(13,3);
				lcd_puts(":");
				
				lcd_gotoxy(10,3);
				lcd_puts(":");
				
				/*lcd_gotoxy(1,0);
				lcd_puts("    ");
				lcd_gotoxy(1,0);
				lcd_count_16(pause);*/			//Ausgabe von lowtime
				if(sec<10)
				{
				lcd_gotoxy(14,3);
				lcd_puts("  ");
				lcd_gotoxy(15,3);
				lcd_count_16(sec);
				}
				else
				{
				lcd_gotoxy(14,3);
				lcd_puts("  ");
				lcd_gotoxy(14,3);
				lcd_count_16(sec);			//Sekunden ausgabe
				}
				
			}	
			else			//negative Flanke
			{
				pause = 0;
				
				
				/*lcd_gotoxy(1,1);
				lcd_puts("   ");
				lcd_gotoxy(1,1);
				lcd_count_16(pruefer);*/			//Ausgabe von der hightime
				
				sec++;
				
				switch(sec)
				{
				
				case 23: min = 0;
						if(pruefer>600)			//Logisches eins
							{
							min = min + 1;
							}
						break;
				case 24: if(pruefer>600)			//Logisches eins
							{
							min = min + 2;
							}
						break;
				case 25: if(pruefer>600)			//Logisches eins
							{
							min = min + 4;
							}
						break;
				case 26: if(pruefer>600)			//Logisches eins
							{
							min = min + 8;
							}
						break;
				case 27: if(pruefer>600)			//Logisches eins
							{
							min = min + 10;
							}
						break;
				case 28: if(pruefer>600)			//Logisches eins
							{
							min = min + 20;
							}
						break;
				case 29: if(pruefer>600)			//Logisches eins
							{
							min = min + 40;
							}
						break;
						
						
				case 31: hour = 0;
						 if(pruefer>600)			//Logisches eins
							{
							hour = hour + 1;
							}
						break;
				case 32: if(pruefer>600)			//Logisches eins
							{
							hour = hour + 2;
							}
						break;
				case 33: if(pruefer>600)			//Logisches eins
							{
							hour = hour + 4;
							}
						break;
				case 34: if(pruefer>600)			//Logisches eins
							{
							hour = hour + 8;
							}
						break;
				case 35: if(pruefer>600)			//Logisches eins
							{
							hour = hour + 10;
							}
						break;
				case 36: if(pruefer>600)			//Logisches eins
							{
							hour = hour + 20;
							}
						break;
						
						
				case 38: day = 0;
						 if(pruefer>600)			//Logisches eins
							{
							day = day + 1;
							}
						break;
				case 39: if(pruefer>600)			//Logisches eins
							{
							day = day + 2;
							}
						break;
				case 40: if(pruefer>600)			//Logisches eins
							{
							day = day + 4;
							}
						break;
				case 41: if(pruefer>600)			//Logisches eins
							{
							day = day + 8;
							}
						break;
				case 42: if(pruefer>600)			//Logisches eins
							{
							day = day + 10;
							}
						break;
				case 43: if(pruefer>600)			//Logisches eins
							{
							day = day + 20;
							}
						break;
						
						
						
				case 44: weekday = 8;
						 if(pruefer>600)			//Logisches eins
							{
							weekday = weekday + 1;
							}
						break;
				case 45: if(pruefer>600)			//Logisches eins
							{
							weekday = weekday + 2;
							}
						break;
				case 46: if(pruefer>600)			//Logisches eins
							{
							weekday = weekday + 4;
							}
						break;
						
						
						
				case 47: month = 0;
						 if(pruefer>600)			//Logisches eins
							{
							month = month + 1;
							}
						break;
				case 48: if(pruefer>600)			//Logisches eins
							{
							month = month + 2;
							}
						break;
				case 49: if(pruefer>600)			//Logisches eins
							{
							month = month + 4;
							}
						break;
				case 50: if(pruefer>600)			//Logisches eins
							{
							month = month + 8;
							}
						break;
				case 51: if(pruefer>600)			//Logisches eins
							{
							month = month + 10;
							}
						break;
						
						
				case 52: year = 0;
						 if(pruefer>600)			//Logisches eins
							{
							year = year + 1;
							}
						break;
				case 53: if(pruefer>600)			//Logisches eins
							{
							year = year + 2;
							}
						break;
				case 54: if(pruefer>600)			//Logisches eins
							{
							year = year + 4;
							}
						break;
				case 55: if(pruefer>600)			//Logisches eins
							{
							year = year + 8;
							}
						break;
				case 56: if(pruefer>600)			//Logisches eins
							{
							year = year + 10;
							}
						break;
				case 57: if(pruefer>600)			//Logisches eins
							{
							year = year + 20;
							}
						break;
				case 58: if(pruefer>600)			//Logisches eins
							{
							year = year + 10;
							}
						break;
				case 59: if(pruefer>600)			//Logisches eins
							{
							year = year + 20;
							}
						break;
				
				}
			}		
			}
	}
return 0;
}