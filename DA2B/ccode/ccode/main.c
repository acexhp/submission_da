/*
 * ccode.c
 * Author : hierholz
 */ 

#define F_CPU 16000000UL		// clock runs at 16 MHz
#include <avr/io.h>
#include <util/delay.h>			
#include <avr/interrupt.h>

int main(void)
{
	DDRB |= (1<<5);		//PB5 an output (PB2/3/4 led is burnout)
	PORTB |= 1<<5;		//led PB5 off
	PORTD |= 1<<2;		//pull up activated
	EICRA = 0x2;			//make INTO falling edge triggered
	EIMSK = (1<<INT0);		//enable external interrupt 0
	sei ();			//enable interrupts
	
    while (1) 
    {					//wait for interruption
    }
}

ISR (INT0_vect)			//ISR for external interrupt 0
{
	PORTB ^= (1<<5);		//toggle PORTB.5
	_delay_ms(1250);
}
