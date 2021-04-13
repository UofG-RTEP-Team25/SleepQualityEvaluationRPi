#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include "i2c.cpp"
#include "ADXL345.h"

using namespace std;



class ADXL{
		public:

		struct acc_dat{
		int x;
		int y;
		int z;
		};
		IIC iic;
		void move_detect()
		{
			int val = 0;
			val =  iic.i2c_read_reg8_singleByte(12 , 13 ,0xA6 , 0xA7 , INT_SOURCE);
			val &= 0x18;
			if((val & 0x18) == 0x10) cout << "active" << "\n";
			else if((val & 0x18) == 0x08) cout << "inactive" << "\n";
			else cout << "erro" << "\n";

		}
		void adxl345_init()
		{
			iic.i2c_init(12 , 13);
			
			iic.i2c_read_reg8_singleByte(12 , 13 , 0XA6 , 0xA7 , INT_SOURCE) ;
			iic.i2c_write_reg8(12 , 13 , 0xA6 , INT_ENABLE , 0x00);
			iic.i2c_write_reg8(12 , 13 , 0xA6 , DATA_FORMAT , 0x0b);
			iic.i2c_write_reg8(12 , 13 , 0xA6 , POWER_CTL , 0x08);
			iic.i2c_write_reg8(12 , 13 , 0xA6 , BW_RATE , 0x0a);


			//enable movements detect
			iic.i2c_write_reg8(12 , 13 , 0xA6 , THRESH_ACT , 6);
			iic.i2c_write_reg8(12 , 13 , 0xA6 , THRESH_INACT , 4);
			iic.i2c_write_reg8(12 , 13 , 0xA6 , TIME_INACT , 1);
			iic.i2c_write_reg8(12 , 13 , 0xA6 , ACT_INACT_CTL , 0xff);


			iic.i2c_write_reg8(12 , 13 , 0xA6 , THRESH_FF , 0x09);
			iic.i2c_write_reg8(12 , 13 , 0xA6 , TIME_FF , 0x20);



			iic.i2c_write_reg8(12 , 13 , 0xA6 , INT_MAP , 0x00);
			iic.i2c_write_reg8(12 , 13 , 0xA6 , INT_ENABLE , 0x18);
			iic.i2c_write_reg8(12 , 13 , 0xA6 , FIFO_CTL , 0x9f);

			
			pinMode(30 ,INPUT);
			pullUpDnControl(30 , PUD_DOWN);
		}



		struct acc_dat adxl345_read_xyz()
		{
			char x0 , y0 , z0 , x1 , y1 , z1;
			struct acc_dat acc_xyz;

			x0 = 0xff - iic.i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , DATAX0);
			x1 = 0xff - iic.i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , DATAX1);
			y0 = 0xff - iic.i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , DATAY0);
			y1 = 0xff - iic.i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , DATAY1);
			z0 = 0xff - iic.i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , DATAZ0);
			z1 = 0xff - iic.i2c_read_reg8_singleByte(12 , 13 , 0xA6 , 0xA7 , DATAZ1);

			acc_xyz.x = (int)(x1 << 8) + (int)x0;
			acc_xyz.y = (int)(y1 << 8) + (int)y0;
			acc_xyz.z = (int)(z1 << 8) + (int)z0;

			return acc_xyz;


		}
		
		
};









/*

int main()
{
	struct acc_dat acc_xyz;

	wiringPiSetup();
	i2c_init(12 , 13);
	adxl345_init();
	pinMode(30 ,INPUT);
	pullUpDnControl(30 , PUD_DOWN);
        if(wiringPiISR(30 , INT_EDGE_RISING , &move_detect) < 0) cout << "999";
	while(1)
	{
	   acc_xyz = adxl345_read_xyz()
		printf("x: %05d  y: %05d  z: %05d\n", acc_xyz.x, acc_xyz.y, acc_xyz.z);
	}
}
*/
