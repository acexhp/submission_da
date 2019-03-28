/*
 * DA3A.c
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL)))-1)

//Function declarations
volatile int Count;
void USART_init( unsigned int ubrr );				//calls integere
void USART_TX_string(char *data);					//calls string
char outs[30];
int random_num;										//creates variables
float AVOGADRO_NUM;						
char string[] = "x 10^23 is Avogadro's number :)";	//creates string
char space[] = " ";									//creates space

int main(void)
{
	Count = 0;
	TIMSK0 |= (1<<TOIE0);							//sets interrupt when overflow 
occurs
	sei ();								
	TCCR0A = 0;										//normal mode
	TCCR0B |= (1<<CS02)|(1<<CS00);					//prescaler = 1024
	USART_init(BAUD_PRESCALLER);					//baud prescaler
	USART_TX_string("Printing...\r\n"); 			//shows succesful connection
    while (1);							
}
//int USART (RS-232)
void USART_init( unsigned int ubrr ){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0);				
	UCSR0C = (3 << UCSZ00);				
}
//send string to RS-232
void USART_TX_string(char *data) {
	while (*data != '\0') {
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = *data;
		data++;
	}
}
ISR (TIMER0_OVF_vect){								//timer0 overlfow interrupt
	while (Count < 61){								//waiting
		if ((TIFR0 & 0x01) == 1){					//detects overflow
			TIFR0 = 0X01;							//reset overlfow flag
			Count++;								
		}
	}
	if (Count > 60){								
		USART_TX_string(string);					//prints string
		USART_TX_string(space);						//prints space
				
		random_num = rand();						//creates random number
		AVOGADRO_NUM = 6.022141;					//creates and store float value 
				
		snprintf(outs, sizeof(outs), "%3d\r\n", random_num);
		USART_TX_string(outs);
		USART_TX_string(space);
		
		sprintf(outs, "%f", AVOGADRO_NUM);		
		USART_TX_string(outs);			
		USART_TX_string(space);
		Count = 0;
	}
}
