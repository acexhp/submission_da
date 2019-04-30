/*
 * DA4B1.c
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Function Prototypes
void set_register(void);   
void set_adc(void);  

int main(void)
{
	int period;				//variable used to set the period
	
	while (1)
	{
		set_register();     //calls function that sets Timer/Counter control Register 0
		set_adc();			//calls function that sets up adc
		DDRB=0x0F;			//enable output
		OCR0A = ADC;		 
		
		// Rotate stepper motor counterclockwise
		if ((OCR0A>=0)&&(OCR0A<=10))
		{
			period =2 ;		//lowest possible period, highest frequency
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x0c;
			_delay_ms(period);
		}
		//period = 10
		else if((OCR0A>10)&&(OCR0A<=20))
		{
			period=10;
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x0c;
			_delay_ms(period);
		}
		//period = 20
		else if((OCR0A>20)&&(OCR0A<=30))
		{
			period=20;
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x0c;
			_delay_ms(period);
		}
		//period = 30
		else if((OCR0A>30)&&(OCR0A<=40))
		{
			period=30;
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x0c;
			_delay_ms(period);
		}
		
		//period = 40
		else if((OCR0A>40)&&(OCR0A<=50))
		{
			period=40;
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x0c;
			_delay_ms(period);
		}
		//period = 50, high period, low frequency
		else if((OCR0A>50)&&(OCR0A<=60))
		{
			period=50;
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x0c;
			_delay_ms(period);
		}
		
		else if(OCR0A>60)			//stepper motor turns off
		{
			PORTB=0;
		}
	}
}

void set_register(void)
{
	TCCR0A |= (1<<COM0A1)|(1<<WGM01); //Sets to CTC mode
	TCCR0B |= (1<<CS02)|(1<<CS00);	  //Pre-scaler is set to 1024
	
	
}

//ADC set up
void set_adc(void)
{
	ADCSRA |= ((1<<ADEN)|(1<<ADSC)|(1<<ADPS1)|(1<<ADPS0)); //adc enable, start conversion, scaler
	ADMUX |= (1<<REFS0);								   //Reads external capacitor
}
