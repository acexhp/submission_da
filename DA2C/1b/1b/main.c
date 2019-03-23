/*
 * 1b.c
 */ 

#define F_CPU 16000000UL				//clock runs at 16 MHz
#include <avr/io.h>
#include<util/delay.h>

int overflow = 0;					//initialize overflow

int main()
{
	DDRB |= (1<<2); 				//set PORTB2 as output
	PORTB |= (1<<2); 				//Turn LED off
	DDRC &= (0<<2);				// set PORTC1 for input
	PORTC |= (1<<2); 				// enable pull-up

	TCCR0A = 0;
	TCCR0B = (1 << CS02) | (1 << CS00);	//pre-scaler = 1024

	//when the PINC is pressed, LED pulses
	while (1) {
		if (!(PINC & (1<<PINC1)))
		{
			overflow = 0;
			TCNT0 = 0;
		}
		
		while ((TIFR0 & 0x01) == 0);//detects overflow
		TCNT0 = 0x00;			//resets counter
		TIFR0 = 0x01;			//reset overflow flag
		overflow++;			//inc overflow
		if (overflow <= 67)		//led turns on
		PORTB = (0 << DDB2);
		else PORTB = (1 << DDB2);	//led off
	}
	
	return 0;
}

