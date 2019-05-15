/*
 * DA6.c
 */ 



#define F_CPU 16000000UL	//Define CPU clock Frequency

//Headers
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "MPU6050_define.h"
#include "I2C_MasterH.h"
#include "USART_RS232_H.h"

//Variables
float acceleration_x,acceleration_y,acceleration_z;
float gyro_x,gyro_y,gyro_z;

int main()
{ //Initialize I2C, MPU6050, USART, Baud rate = 9600
	float a_x,a_y,a_z;
	float X_g=0,Y_g=0,Z_g=0; 
	char buffer[20], float_[10];
	I2C_Init();											
	MPU6050_init();						
	USART_Init(9600);									
	
	while(1)
	{
		Read_Raw_Value();	
		
		//Divide values by scale factor
		a_x = acceleration_x/16384.0;
		a_y = acceleration_y/16384.0;
		a_z = acceleration_z/16384.0;
		
		X_g = gyro_x/16.4;
		Y_g = gyro_y/16.4;
		Z_g = gyro_z/16.4;
																		
		//Values sent to USART
		dtostrf( a_x, 3, 2, float_ );
		sprintf(buffer," acceleration_x = %s g\t",float_);
		USART_SendString(buffer);

		dtostrf( a_y, 3, 2, float_ );
		sprintf(buffer," acceleration_y = %s g\t",float_);
		USART_SendString(buffer);
		
		dtostrf( a_z, 3, 2, float_ );
		sprintf(buffer," acceleration_z = %s g\t",float_);
		USART_SendString(buffer);

		dtostrf( X_g, 3, 2, float_ );
		sprintf(buffer," gyro_X_axis = %s%c/s\t",float_,0xF8);
		USART_SendString(buffer);

		dtostrf( Y_g, 3, 2, float_ );
		sprintf(buffer," gyro_Y_axis = %s%c/s\t",float_,0xF8);
		USART_SendString(buffer);
		
		dtostrf( Z_g, 3, 2, float_ );
		sprintf(buffer," gyro_Z_axis = %s%c/s\r\n",float_,0xF8);
		USART_SendString(buffer);
		
		_delay_ms(1000);
	}
}

//Read values from gyro
void Read_Raw_Value()	
{
	MPU_Start_Loc();
	acceleration_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	acceleration_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	acceleration_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	gyro_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	gyro_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	gyro_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	I2C_Stop();
}

void MPU6050_init()				//Initialize gyroscope
{
	_delay_ms(150);				//Delay time
	I2C_Start_Wait(0xD0);	
	I2C_Write(SMPLRT_DIV);		//Write sample rate register
	I2C_Write(0x07);			//Set 1KHz 
	I2C_Stop();
	I2C_Start_Wait(0xD0);
	I2C_Write(PWR_MGMT_1);		//Write to power management register
	I2C_Write(0x01);			//X-axis ref freq
	I2C_Stop();
	I2C_Start_Wait(0xD0);
	I2C_Write(CONFIG);			//Write to Configuration register
	I2C_Write(0x00);			//Fs = 8KHz
	I2C_Stop();
	I2C_Start_Wait(0xD0);
	I2C_Write(GYRO_CONFIG);		//Write to Gyroscope config. register
	I2C_Write(0x18);			//Full scale range +/- 2000 degree/C
	I2C_Stop();
	I2C_Start_Wait(0xD0);
	I2C_Write(INT_ENABLE);		//Write interrupt enable register
	I2C_Write(0x01);
	I2C_Stop();
}

void MPU_Start_Loc()
{
	I2C_Start_Wait(0xD0);		//I2C start with device write address
	I2C_Write(ACCEL_XOUT_H);	//Write start location address from where to read
	I2C_Repeated_Start(0xD1);	//I2C start with device read address
}


