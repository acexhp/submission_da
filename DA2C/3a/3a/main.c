/*
 * 3a.c
 */ 

#define F_CPU 16000000UL				//clock runs at 16 MHz
#include <avr/io.h>
#include<avr/interrupt.h>

int overflow = 0;						//initialize overflow

int main(void)
{
	DDRB |= (1 << DDB2);				//PB2 as output
	TCNT0 = 0;							//start counter
	OCR0A = 255;						//load compare reg value
	TCCR0A |= (1 << WGM01);				//set to ctc mode
	TIMSK0 |= (1 << OCIE0A);			//set interrupt on compare match
	TCCR0B = (1 << CS02) | (1 << CS00);	//pre-scaler = 1024
	sei();								//enables interrupt
	while (1)
	{
		
	}
}

ISR (TIMER0_COMPA_vect)					//timer0 overflow interrupt
{
		while ((TIFR0 & 0x02) == 0);	//detects overflow
		TCNT0 = 0x00;					//resets counter
		TIFR0 = 0x02;					//reset overflow flag
		overflow++;						//inc overflow
		if (overflow <= 13)				//led on (13 instead of 26 because...
		PORTB = (0 << DDB2);			//... overflow is being doubled)
		else PORTB = (1 << DDB2);		
		if (overflow == 22) {
			overflow = 0;				//turns off led
		}
}