/*
 * DA5.c
 */ 

//clock frequency
#ifndef F_CPU
#define F_CPU 16000000UL
#endif


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//Set up UART for printf();
#ifndef BAUD
#define BAUD 9600
#endif
#include "inc\STDIO_UART.c"

//Include nRF24L01+ library
#include "inc\nrf24l01.c"
#include "inc\nrf24l01-mnemonics.h"
#include "inc\spi.c"
void print_config(void);

//Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;

void set_adc (void);
volatile unsigned char temperature_value[3];
volatile uint8_t adc_num;



int main(void)
{
	

	set_adc();

		
	//Set message to send (message < 32 characters)
	char tx_message[32];				// Define string array
	strcpy(tx_message,"Hello World!");	// String into array
	
	//	Initialize UART
	uart_init();
	
	//	Initialize nRF24L01+ and print configuration info
	nrf24_init();
	print_config();
	
	//	Start listening to incoming messages
	nrf24_start_listening();
	
		strcpy(tx_message,"Hello World!");	//Copying string to array
	nrf24_send_message(tx_message);
	
	while (1)
	{
		if (message_received)
		{
			//Message received, print it
			message_received = false;
			printf("Received message: %s\n",nrf24_read_message());
			//Send message as response
			_delay_ms(500);
			status = nrf24_send_message(temperature_value);
			if (status == true) printf("Message sent successfully\n");
		}
	}
}

//Interrupt on IRQ pin
ISR(INT0_vect)
{
	message_received = true;
}

void print_config(void)
{
	uint8_t data;
	printf("Startup successful\n\n nRF24L01+ configured as:\n");
	printf("-------------------------------------------\n");
	nrf24_read(CONFIG,&data,1);
	printf("CONFIG		0x%x\n",data);
	nrf24_read(EN_AA,&data,1);
	printf("EN_AA			0x%x\n",data);
	nrf24_read(EN_RXADDR,&data,1);
	printf("EN_RXADDR		0x%x\n",data);
	nrf24_read(SETUP_RETR,&data,1);
	printf("SETUP_RETR		0x%x\n",data);
	nrf24_read(RF_CH,&data,1);
	printf("RF_CH			0x%x\n",data);
	nrf24_read(RF_SETUP,&data,1);
	printf("RF_SETUP		0x%x\n",data);
	nrf24_read(STATUS,&data,1);
	printf("STATUS		0x%x\n",data);
	nrf24_read(FEATURE,&data,1);
	printf("FEATURE		0x%x\n",data);
	printf("-------------------------------------------\n\n");
}


//  Interrupt used to follow instructions below when conversion is done
ISR(ADC_vect)
{
	
	volatile unsigned int i=0;		// From characters to string
	char temperature[3];
	
	adc_num = (ADCL >> 1); 			//Shifts the value right to one place
	
	itoa(adc_num, temperature, 10);	 //Int to string
	
	
	while (i<3)						//Making string from characters of temperature_value
	{
		temperature_value[i] = temperature[i];
		i++;
	}
}

void set_adc (void)
{
	ADMUX = 0x40;	//ADC shifts right
	ADCSRA = 0xEF;  //ADC enable, start conversion, auto trig enable, interrupt enable, set prescaler
}
