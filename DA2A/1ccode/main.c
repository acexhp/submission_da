/*
 * 1ccode.c
 * Author : hierholz
 */ 


#define F_CPU 16000000UL			/* clock runs at 16 MHz*/

#include <avr/io.h>					
#include <util/delay.h>				/* library that is needed to be able to use 
									certain functions*/
int main(void)
{
    DDRB = 0x04;					/* pin 2 in port B an output */
	
    while(1) 
    {
		PORTB = (1 << PORTB2);		/* displays output */
		_delay_ms(435);				/* time that the led is off (60% DC with 0.725s 
									period) */
		PORTB = ~(1 << PORTB2);		/* displays output */
		_delay_ms(290);				/* time that the led is on (40% DC with 0.725s 
									period) */
    }
	return 0;
}





