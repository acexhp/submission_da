/*
 * 2ccode.c
 * Author : hierholz
 */ 

#define F_CPU 16000000UL   				/* clock runs at 16 MHz*/

#include <avr/io.h>				
#include <util/delay.h>					/* library that is needed to be able to use 
										certain functions*/

int main(void)
{
   	DDRB |= (1<<2); 					/* set PORTB2 as output */
	PORTB |= (1<<2); 					/* Turn LED off */
	
	DDRC &= (0<<2);						/* set PORTC1 for input */
	PORTC |= (1<<2); 					/* enable pull-up */

/* when the PINC is pressed, LED pulses for 1.25s */
	while (1) {
		if (!(PINC & (1<<PINC1)))
		{
			PORTB &= ~(1<<2);
			_delay_ms(1250);	 
		}
		else 
			PORTB |= (1<<2);
		}
    
	return 0;
}
