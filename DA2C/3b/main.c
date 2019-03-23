/*
 * 3b.c
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
while (1) {
	if (!(PINC & (1<<PINC1)))
	{
		overflow = 0;
		TCNT0 = 0;
	}
	
	while ((TIFR0 & 0x02) == 0);		//detects overflow
	TCNT0 = 0x00;						//resets counter
	TIFR0 = 0x02;						//reset overflow flag
	overflow++;							//inc overflow
	if (overflow <= 69)					//led turns on
	PORTB = (0 << DDB2);
	else PORTB = (1 << DDB2);			//led off
}
	
}
