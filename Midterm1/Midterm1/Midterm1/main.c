/*
 * Midterm1.c
 */ 


#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALLER F_CPU

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int count_ovfl=0;				//counter for overflow


void usart_init ();
void usart_send (unsigned char ch);
void usart_print(char* str);
void read_adc(void);			//Reads LM34
volatile int adc_temp;			//temperature variable
char outs[256];					//String sending USART commands
volatile char received_data;		//Receiving communication

int main (void)
{
	usart_init ();
	
	/** Setup and enable ADC **/
	ADMUX = (0<<REFS1)|			// Reference Selection Bits
	(1<<REFS0)|					// AVcc - external cap at AREF
	(0<<ADLAR)|					// ADC Left Adjust Result
	(1<<MUX2)|					// Analog Channel Selection Bits
	(0<<MUX1)|					// ADC5 (PC5 PIN27)
	(1<<MUX0);
	ADCSRA = (1<<ADEN)|			// ADC ENable
	(0<<ADSC)|					// ADC Start Conversion
	(0<<ADATE)|					// ADC Auto Trigger Enable
	(0<<ADIF)|					// ADC Interrupt Flag
	(0<<ADIE)|					// ADC Interrupt Enable
	(1<<ADPS2)|					// ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);
	
	
	
	TIMSK0 |= (1<<TOIE0);
	TCNT0 = 0;					// setting initial value for counter
	sei();						// enable global interrupts
	TCCR0B |=(1<<CS02);			// setting prescaler to 256
	
	usart_init(BAUD_PRESCALLER);	//delay for starting hardware
	_delay_ms(500);
	
	
	while (1)					//Reads temp, prints to Thingspeak
	{
		char AR[]= "AT\r\n";
		char AT_CWMODE[]= "AT+CWMODE=1\r\n";	//Sets station mode
		char AT_CWJAP[]= "AT+CWJAP=\"Wifi\",\"password\"\r\n"; //Wifi and password 
		char AT_CIPMUX= "AT+CIPMUX=0\r\n";		//IP address
		char AT_CIPSTART[]= "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";	//Connect to Thingspeak
		char AT_CIPSEND= "AT+CIPSEND=100\r\n";
		
		_delay_ms(200);
		usart_send(AT);
		_delay_ms(5000);
		usart_send(AT_CWMODE);
		_delay_ms(5000);
		usart_send(AT_CWJAP);
		_delay_ms(3000);
		usart_send(AT_CIPMUX);
		_delay_ms(3000);
		usart_send(AT_CIPSTART);
		_delay_ms(3000);
		usart_send(AT_CIPSEND);
		_delay_ms(5000);
		
		PORTC^=(1<<5);
		read_temp();
		
		if (count_ovfl==61)			// data is updated if overflow is 61 times
		{
			int a = ADCL;
			a = a | (ADCH<<8);
			a = (a/1024.0) * 5000/10;
			usart_send((a/100)+'0');
			a = a % 100;
			usart_send((a/10)+'0');
			a = a % 10;
			usart_send((a)+'0');
			usart_send('\r');
			count_ovfl=0;
		}
		
	}
	return 0;
}



ISR(TIMER0_OVF_vect)
{
	while (!(TIFR0 & 0X01)==0);
	count_ovfl++;				//increasing overflow counter
	TCNT0=0X00;					//resetting counter to zero
	TIFR0=0X01;					// reset the overflow flag
	
	
}


void usart_init (void)			//transmit, receive, interrupt	
{					
	UBRR0H=(unsigned char);
	UBRROL=(unsigned char);
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
	UBRR0L = F_CPU/16/BAUD_RATE-1;
}

void usart_send (unsigned char ch)
{
	while (! (UCSR0A & (1<<UDRE0))); 		//wait until UDR0 is empty
	UDR0 = ch;					//transmit ch
}

void usart_print(char* str)
{
	int i = 0;
	while(str[i] != 0)
	usart_send(str[i]);
}

