/*
 * DA4B2.c
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

//Function Prototypes
void set_register(void);	
void set_adc(void);			


int main(void)
{
	
	while (1)
	{
		set_register();     //calls function that sets Timer/Counter control Register 0
		set_adc();			//calls function that sets up adc
		ICR1=4999;			//sets top register
		DDRB|=(1<<PB1);		//output
		OCR1A=ADC;			//controls servo, reads from PB1
		_delay_ms(100);		//controls degree 
	}
}

void set_register(void)
{
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);			//sets Mode = 14, Fast PWM
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);	//Prescaler = 64
}


//ADC set up
void set_adc(void)
{
	ADCSRA |= ((1<<ADEN)|(1<<ADSC)|(1<<ADPS1)|(1<<ADPS0)); //adc enable, start conversion, scaler
	ADMUX |= (1<<REFS0);								   //Reads external capacitor
}


