/*
 * 2a.c
 */ 

#define F_CPU 16000000UL				//clock runs at 16 MHz
#include <avr/io.h>
#include<avr/interrupt.h>

int overflow = 0;						//initialize overflow

int main(void)
{
	DDRB |= (1 << DDB2);				//PB2 as output
	TIMSK0 |= (1 << TOIE0);				//enables interrupt
	TCNT0 = 0;							//start counter
	sei();								//enables interrupt
	TCCR0B = (1 << CS02) | (1 << CS00);	//pre-scaler = 1024
	while (1)
	{
		
	}
}

ISR (TIMER0_OVF_vect)					//timer0 overflow interrupt
{
		while ((TIFR0 & 0x01) == 0);	//detects overflow
		TCNT0 = 0x00;					//resets counter
		TIFR0 = 0x01;					//reset overflow flag
		overflow++;						//inc overflow
		if (overflow <= 13)				//led on (13 instead of 26 because...
		PORTB = (0 << DDB2);			//... overflow is being doubled)
		else PORTB = (1 << DDB2);		
		if (overflow == 22) {
			overflow = 0;				//turns off led
		}
}