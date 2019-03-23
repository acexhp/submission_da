/*
 * 1a.c
 */ 

#define F_CPU 16000000UL				//clock runs at 16 MHz
#include <avr/io.h>
#include<util/delay.h>

int main()
{
	int overflow = 0;					//initialize overflow
    DDRB |= (1 << DDB2);				//PB2 as output
	TCCR0A = 0;
	TCNT0 = 0x00;						//start timer
	TCCR0B = (1 << CS02) | (1 << CS00);	//pre-scaler = 1024

	
	while (1){
		while ((TIFR0 & 0x01) == 0);	//detects overflow
		TCNT0 = 0x00;					//resets counter
		TIFR0 = 0x01;					//reset overflow flag
		overflow++;						//inc overflow
		if (overflow <= 26)				//led on
			PORTB = (0 << DDB2);		
			else PORTB = (1 << DDB2);
		if (overflow == 44) {
			overflow = 0;				//turns off led		
		}
	}
}

