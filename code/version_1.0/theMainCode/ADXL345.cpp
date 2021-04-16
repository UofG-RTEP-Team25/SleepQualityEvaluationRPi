#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include "i2c.h"
#include "ADXL345.h"

using namespace std;

extern int mov;


struct acc_dat{
	int x;
	int y;
	int z;
};

void move_detect()
{
	int val = 0;
	val =  i2c_read_reg8_singleByte(12 , 13 ,0xA6 , 0xA7 , INT_SOURCE);
	val &= 0x18;
	if((val & 0x18) == 0x10) mov = 1;
	else if((val & 0x18) == 0x08) mov = 0;

}



void adxl345_init()
{
	i2c_init(12 , 13);
	i2c_read_reg8_singleByte(12 , 13 , 0XA6 , 0xA7 , INT_SOURCE) ;
	i2c_write_reg8(12 , 13 , 0xA6 , INT_ENABLE , 0x00);
	i2c_write_reg8(12 , 13 , 0xA6 , DATA_FORMAT , 0x0b);
	i2c_write_reg8(12 , 13 , 0xA6 , POWER_CTL , 0x08);
	i2c_write_reg8(12 , 13 , 0xA6 , BW_RATE , 0x0a);

	//enable movements detect
	i2c_write_reg8(12 , 13 , 0xA6 , THRESH_ACT , 6);
	i2c_write_reg8(12 , 13 , 0xA6 , THRESH_INACT , 4);
	i2c_write_reg8(12 , 13 , 0xA6 , TIME_INACT , 1);
	i2c_write_reg8(12 , 13 , 0xA6 , ACT_INACT_CTL , 0xff);

	i2c_write_reg8(12 , 13 , 0xA6 , THRESH_FF , 0x09);
	i2c_write_reg8(12 , 13 , 0xA6 , TIME_FF , 0x20);
	i2c_write_reg8(12 , 13 , 0xA6 , INT_MAP , 0x00);
	i2c_write_reg8(12 , 13 , 0xA6 , INT_ENABLE , 0x18);
	i2c_write_reg8(12 , 13 , 0xA6 , FIFO_CTL , 0x9f);
}


struct acc_dat adxl345_read_xyz()
{
	char x0 , y0 , z0 , x1 , y1 , z1;
	struct acc_dat acc_xyz;

	x0 = 0xff - i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , DATAX0);
	x1 = 0xff - i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , DATAX1);
	y0 = 0xff - i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , DATAY0);
	y1 = 0xff - i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , DATAY1);
	z0 = 0xff - i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , DATAZ0);
	z1 = 0xff - i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , DATAZ1);
	acc_xyz.x = (int)(x1 << 8) + (int)x0;
	acc_xyz.y = (int)(y1 << 8) + (int)y0;
	acc_xyz.z = (int)(z1 << 8) + (int)z0;

	return acc_xyz;


};
		
		



