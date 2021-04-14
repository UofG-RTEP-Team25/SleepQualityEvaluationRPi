#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include "i2c.h"
#include "MAX44009.h"



using namespace std;




void max44009_init()
{
	i2c_read_reg8_singleByte( 21 , 22 , WRITE_ADDR_GND , READ_ADDR_GND , INTR_STATUS);

	i2c_write_reg8( 21 , 22 , WRITE_ADDR_GND , REG_CONFIG , 0X00);
	i2c_write_reg8( 21 , 22 , WRITE_ADDR_GND , INTR_ENABLE , 0x00);
}


void max44009_read_light()
{
	int i , index = 0 , num = 0 , temp_num = 0 , sqrt = 1;
	double lux;

	temp_num = i2c_read_reg8_singleByte( 21 , 22 , WRITE_ADDR_GND , READ_ADDR_GND , REG_LUX_HIGH);
	index = (temp_num & 0xf0) >> 4;
	for(i = 0;i < index;i++)
	{
		sqrt *= 2;
	}
	num = temp_num & 0x0f;
	lux = sqrt * num * 0.72;
	cout << index << "\n";
	cout << num << "\n";
	cout << "lux:" << lux <<"\n" << "\n";

}


void sound_detect()
{
	cout << "y" << "\n";
}




