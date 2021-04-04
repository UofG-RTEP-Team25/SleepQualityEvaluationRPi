#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include "i2c.cpp"

using namespace std;


struct acc_dat{
	int x;
	int y;
	int z;

};


void adxl345_init()
{
	i2c_write_reg8(12 , 13 , 0xA6 , 0x31 , 0x0b);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x2d , 0x08);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x1e , 0x00);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x1f , 0x00);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x20 , 0x00);

	i2c_write_reg8(12 , 13 , 0xA6 , 0x21 , 0x00);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x22 , 0x00);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x23 , 0x00);

	i2c_write_reg8(12 , 13 , 0xA6 , 0x24 , 0x01);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x25 , 0x0f);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x26 , 0x2b);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x27 , 0x00);


	i2c_write_reg8(12 , 13 , 0xA6 , 0x28 , 0x09);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x29 , 0xff);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x2a , 0x80);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x2c , 0x0a);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x2f , 0x00);
	i2c_write_reg8(12 , 13 , 0xA6 , 0x38 , 0x9f);

}



struct acc_dat adxl345_read_xyz()
{
	char x0 , y0 , z0 , x1 , y1 , z1;
	struct acc_dat acc_xyz;

	x0 = 0xff - i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , 0x32);
	x1 = 0xff - i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , 0x33);
	y0 = 0xff - i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , 0x34);
	y1 = 0xff - i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , 0x35);
	z0 = 0xff - i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , 0x36);
	z1 = 0xff - i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , 0x37);

	acc_xyz.x = (int)(x1 << 8) + (int)x0;
	acc_xyz.y = (int)(y1 << 8) + (int)y0;
	acc_xyz.z = (int)(z1 << 8) + (int)z0;

	return acc_xyz;


}



int main()
{
	struct acc_dat acc_xyz;

	wiringPiSetup();
	i2c_init(12 , 13);
	adxl345_init();

	while(1)
	{
		acc_xyz = adxl345_read_xyz();
		cout << "  x: " << acc_xyz.x << "  y: " << acc_xyz.y << "  z: " << acc_xyz.z   << "\n";

		delay(1000);
	}
}
