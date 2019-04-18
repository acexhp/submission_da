/*
 * DA4A.c
 */ 

#define F_CPU 16000000UL		//clock runs at 16 MHz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define MTR_1 5
#define SWITCH (PINC&(1<<3))    // switch on PINC3

int DCMotor=0;					//variable for on/off motor

int main(void)
{
	
	
	PORTC |=(1<<2);				//pull-up resistor
	OCR0A = 200;				//setting top = 200, frequency
	
	//sets timer/counter control register, mode 7, fast PWM and prescaler is 256
	TCCR0A |= (1<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(0<<COM0B0)|(1<<WGM01)|(1<<WGM00);
	TCCR0B |= (1<<WGM02)|(1<<CS02)|(0<<CS01)|(0<<CS00);
	
	while (1)
	{
		//Checks if the button is pressed while motor is off
		while (!DCMotor)
		{
			if(!SWITCH)
			{
				_delay_ms(100);
				
				ADCSRA |= ((1<<ADEN)|(1<<ADSC)|(0<<ADPS2)|(0<<ADPS1)|(1<<ADPS0));
				ADMUX |= (1<<REFS0);
				
				{
					OCR0B = ADC;   //value being read from potentiometer, sets duty cycle
					DDRD = 0xFF;   //output
					PORTD = 0x20;  //sets PD5 HIGH
					DCMotor=1;	   // setting motor to 1 to get out of while loop
				}
			}
		}
		
		//Checks if the button is pressed while motor is on
		while (DCMotor)
		{
			if(!SWITCH)
			{
				_delay_ms(100);
				
				// Turns off the motor and let it run CW
				PORTD &= ~(1<<5);
				DDRD = 0;
				DCMotor=0;
			}
			
		}
	}
}


