#include <wiringPi.h>
#include <stdio.h>
#include "i2c.h"

void i2c_init(char scl, char sda)
{
	wiringPiSetup();
	pinMode( scl, OUTPUT);
	pinMode( sda, OUTPUT);
        digitalWrite(scl, HIGH);
	digitalWrite(sda, HIGH);
	delayMicroseconds(10);

}


void i2c_start(char scl, char sda)
{
	SDA_OUT(sda);
        digitalWrite(scl, HIGH);
	digitalWrite(sda, HIGH);
	delayMicroseconds(4);
	digitalWrite(sda, LOW);
	delayMicroseconds(4);
	digitalWrite(scl, LOW);


}

void i2c_stop(char scl, char sda)
{
	SDA_OUT(sda);
        digitalWrite(scl, LOW);
	digitalWrite(sda, LOW);
	delayMicroseconds(4);
	digitalWrite(scl,HIGH);
	delayMicroseconds(2);
	digitalWrite(sda, HIGH);	
	delayMicroseconds(4);

}



unsigned char i2c_wait_ack(char scl, char sda)
{
	unsigned char ErrTime = 0;
	SDA_IN(sda);
	digitalWrite(sda, HIGH);
	delayMicroseconds(1);
	digitalWrite(scl, HIGH);

	while(digitalRead(sda))
	{
		ErrTime++;
		if(ErrTime > 250)
		{
			i2c_stop(scl, sda);
			return 1;
		}
	}
	digitalWrite(scl, LOW);
	return 0;

}


void i2c_ack(char scl, char sda)
{
	digitalWrite(scl, LOW);
	SDA_OUT(sda);
	digitalWrite(sda, LOW);
	delayMicroseconds(2);
	digitalWrite(scl, HIGH);
	delayMicroseconds(2);
	digitalWrite(scl, LOW);

}


void i2c_nack(char scl, char sda)
{
	digitalWrite(scl, LOW);
	SDA_OUT(sda);
	digitalWrite(sda, HIGH);
	delayMicroseconds(2);
	digitalWrite(scl, HIGH);
	delayMicroseconds(2);
	digitalWrite(scl, LOW);

}


void i2c_send_Byte(char scl, char sda, int data)
{
	int i;
	SDA_OUT(sda);
	digitalWrite(scl, LOW);
	for(i = 0; i < 8; i++)
	{

		digitalWrite(sda, (data & 0x80) >> 7);
		data <<= 1;
		delayMicroseconds(2);
		digitalWrite(scl, HIGH);
                delayMicroseconds(2);
		digitalWrite(scl, LOW);
                delayMicroseconds(2);
	}
}



unsigned char i2c_read_Byte(char scl, char sda, int ack)
{
	int i, ErrTime;
	unsigned char receive = 0;
	SDA_IN(sda);
	for(i = 0; i < 8; i++)
	{
		ErrTime = 0;
		receive <<= 1;
		digitalWrite(scl, LOW);
		delayMicroseconds(2);
		digitalWrite(scl, HIGH);
		while(!digitalRead(sda))
		{
			ErrTime++;
			if(ErrTime > 250) break;
		}
		if(ErrTime < 250) receive++;
		delayMicroseconds(1);
	}
	if(!ack) i2c_nack(scl, sda);
	else i2c_ack(scl, sda);
	return receive;
}



