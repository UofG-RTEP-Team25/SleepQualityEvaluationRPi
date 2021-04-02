#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include<wiringPi.h>
#include<wiringPiI2C.h>
#include"MAX30102.h"
#include"algorithm.cpp"
#include"i2c.cpp"

using namespace std;

int fd , rd_ptr = 0 , ir_data = 0 , tem , sample_num = 0;
uint32_t data_buffer[100] , non_buffer[100];
int32_t n_sp02 , n_heart_rate;
int8_t ch_spo2_valid , ch_hr_valid;
double temp;
unsigned char init_data_2[2] = { REG_INTR_STATUS_1 , REG_INTR_STATUS_2 };



void max30102_init()
{

	/*if( wiringPiI2CWriteReg8( fd , REG_MODE_CONFIG , 0X40) < 0) cout << "1";
	if( wiringPiI2CWriteReg8( fd , REG_INTR_ENABLE_1 , 0X40) < 0)  cout << "2";

	if( wiringPiI2CWriteReg8( fd , REG_INTR_ENABLE_2 , 0X00) < 0)   cout << "3";
	if( wiringPiI2CWriteReg8( fd , REG_FIFO_WR_PTR , 0x00) < 0)	cout << "4";
	if( wiringPiI2CWriteReg8( fd , REG_OVF_COUNTER , 0x00) < 0)	cout << "5";
	if( wiringPiI2CWriteReg8( fd , REG_FIFO_RD_PTR , 0x00) < 0)	cout << "6";
	if( wiringPiI2CWriteReg8( fd , REG_FIFO_CONFIG , 0x4f) < 0)	cout << "7";
	if( wiringPiI2CWriteReg8( fd , REG_MODE_CONFIG , 0X02) < 0)	cout << "8";
	if( wiringPiI2CWriteReg8( fd , REG_SPO2_CONFIG , 0X27) < 0)	cout << "9";
	if( wiringPiI2CWriteReg8( fd , REG_LED1_PA , 0X24) < 0) 	cout << "10";
	if( wiringPiI2CWriteReg8( fd , REG_LED2_PA , 0X24) < 0) 	cout << "11";
	if( wiringPiI2CWriteReg8( fd , REG_PILOT_LED , 0X7F) < 0)	cout << "11";
	if( wiringPiI2CReadReg8( fd, REG_INTR_STATUS_1) < 0)		cout << "12";
	if( wiringPiI2CReadReg8( fd, REG_INTR_STATUS_2) < 0)	cout << "13";*/

        unsigned char reg_buffer[12] = { REG_MODE_CONFIG , REG_INTR_ENABLE_1 , REG_INTR_ENABLE_2 , REG_FIFO_WR_PTR , REG_OVF_COUNTER , REG_FIFO_RD_PTR , REG_FIFO_CONFIG , REG_MODE_CONFIG , REG_SPO2_CONFIG , REG_LED1_PA , REG_LED2_PA , REG_PILOT_LED } , init_data_1[12] = { 0x40 , 0x40 , 0x00 , 0x00 , 0x00 , 0x00 , 0x4f , 0x02 , 0x27 , 0x24 , 0x24 , 0x7f };
	int t, n;


	for(t = 0 ; t < 12; t++)
	{		
		i2c_start(9, 8);
		i2c_send_Byte(9, 8, 0xAE);
		if(!i2c_wait_ack(9, 8)) 
		{
			i2c_send_Byte(9, 8, reg_buffer[t]);
			if(!i2c_wait_ack(9, 8))
			{
				i2c_send_Byte(9, 8, init_data_1[t]);
				if(!i2c_wait_ack(9, 8)) i2c_stop(9, 8);
				else cout << " ack erro 1" << "\n";
			}
			else cout << " ack erro 2" << "\n";
		}
		else cout << " ack erro 3" << "\n";
	}
	for(n = 0; n < 2; n++ )
	{
		i2c_start(9, 8);
		i2c_send_Byte(9, 8, 0xAE);
		if(!i2c_wait_ack(9, 8))
		{
			i2c_send_Byte(9, 8, init_data_2[n]);
			if(!i2c_wait_ack(9, 8))
			{
				i2c_start(9, 8);
				i2c_send_Byte(9, 8,0xAF);
				if(!i2c_wait_ack(9, 8))
				{
					i2c_read_Byte(9, 8, 0);
					i2c_stop(9, 8);
				}
				else cout << " ack erro 4" << "\n";
			}
			else cout << " ack erro 5" << "\n";
		}
		else cout << " ack erro 6" << "\n";
	}


}


    


void max30102_read_hr( void )
{
	int n;

	i2c_start(9, 8);
	i2c_send_Byte(9, 8, 0xAE);
	if(!i2c_wait_ack(9, 8))
	{
		i2c_send_Byte(9, 8, REG_FIFO_DATA);
		if(!i2c_wait_ack(9, 8))
		{
			i2c_start(9, 8);
			i2c_send_Byte(9, 8, 0xAF);
			if(!i2c_wait_ack(9, 8))
			{
				tem = i2c_read_Byte(9, 8, 1) << 16;
				ir_data += tem;
				tem = i2c_read_Byte(9, 8, 1) << 8;
				ir_data += tem;
				tem = i2c_read_Byte(9, 8, 0);
				ir_data += tem;

				i2c_stop(9, 8);
				ir_data &= 0x03ffff;
				data_buffer[sample_num] = ir_data;

			}
			else cout << " ack erro 7" << "\n";
		}
		else cout << " ack erro 8" << "\n";

	}
	else cout << " ack erro 9" << "\n";

	i2c_start(9, 8);
	i2c_send_Byte(9, 8, 0xAE);
	if(!i2c_wait_ack(9, 8)) 
	{
		i2c_send_Byte(9, 8, REG_FIFO_RD_PTR);
		if(!i2c_wait_ack(9, 8))
		{
			i2c_send_Byte(9, 8, rd_ptr);
			if(!i2c_wait_ack(9, 8)) i2c_stop(9, 8);
			else cout << " ack erro 10" << "\n";
		}
		else cout << " ack erro 11" << "\n";
	}
	else cout << " ack erro 12" << "\n";
	if(rd_ptr < 31) rd_ptr++;
	else rd_ptr = 0;




/*	wiringPiI2CWrite( fd , REG_FIFO_DATA);
	tem = wiringPiI2CRead( fd ) << 16;
	ir_data += tem;
	tem = wiringPiI2CRead( fd ) << 8;
	ir_data += tem;
	tem = wiringPiI2CRead( fd );
	ir_data += tem;
	ir_data &= 0x03ffff;
	data_buffer[sample_num] = ir_data;

	if( wiringPiI2CWriteReg8( fd , REG_FIFO_RD_PTR , rd_ptr ) < 0) cout << "21";
	else if( rd_ptr < 31) ++rd_ptr;  else rd_ptr = 0;*/

	if(sample_num < 99) sample_num++;
	else	
	{
		sample_num = 0;
		maxim_heart_rate_and_oxygen_saturation( data_buffer , 100 , non_buffer , &n_sp02 , &ch_spo2_valid , &n_heart_rate , &ch_hr_valid);
		cout << n_heart_rate << "\n";
		
		
		i2c_start(9, 8);
		i2c_send_Byte(9, 8, 0xAE);
		if(!i2c_wait_ack(9, 8)) 
		{
			i2c_send_Byte(9, 8, 0x21);
			if(!i2c_wait_ack(9, 8))
			{
				i2c_send_Byte(9, 8, 0x01);
				if(!i2c_wait_ack(9, 8)) i2c_stop(9, 8);
				else cout << " ack erro 13" << "\n";
			}
			else cout << " ack erro 14" << "\n";
		}
		else cout << " ack erro 15" << "\n";
	
		i2c_start(9, 8);
		i2c_send_Byte(9, 8, 0xAE);
		if(!i2c_wait_ack(9, 8))
		{
			i2c_send_Byte(9, 8, 0x1f);
			if(!i2c_wait_ack(9, 8))
			{
				i2c_start(9, 8);
				i2c_send_Byte(9, 8, 0xAF);
				if(!i2c_wait_ack(9, 8))
				{
					temp = i2c_read_Byte(9, 8, 0);
					i2c_stop(9, 8);
					
				}
				else cout << " ack erro 16" << "\n";
			}
			else cout << " ack erro 17" << "\n";

		}
		else cout << " ack erro 18" << "\n";
               
		i2c_start(9, 8);
		i2c_send_Byte(9, 8, 0xAE);
		if(!i2c_wait_ack(9, 8))
		{
			i2c_send_Byte(9, 8, 0x20);
			if(!i2c_wait_ack(9, 8))
			{
				i2c_start(9, 8);
				i2c_send_Byte(9, 8, 0xAF);
				if(!i2c_wait_ack(9, 8))
				{
					temp += 0.625*i2c_read_Byte(9, 8, 0);
					i2c_stop(9, 8);
					
				}
				else cout << " ack erro 19" << "\n";
			}
			else cout << " ack erro 20" << "\n";

		}
		else cout << "ack erro 21" << "\n";

             
	//	wiringPiI2CWriteReg8( fd,0x21,0x01);
	//	temp = wiringPiI2CReadReg8(fd , 0x1f) + 0.625* wiringPiI2CReadReg8(fd , 0x20);
		cout << "TEMP:" << temp << "\n" << "\n";
	}

//	if( wiringPiI2CReadReg8( fd, REG_INTR_STATUS_1) < 0) cout << "14";
//	if( wiringPiI2CReadReg8( fd, REG_INTR_STATUS_2) < 0)	cout << "15";
	for(n = 0; n < 2; n++ )
	{
		i2c_start(9, 8);
		i2c_send_Byte(9, 8, 0xAE);
		if(!i2c_wait_ack(9, 8))
		{
			i2c_send_Byte(9, 8, init_data_2[n]);
			if(!i2c_wait_ack(9, 8))
			{
				i2c_start(9, 8);
				i2c_send_Byte(9, 8,0xAF);
				if(!i2c_wait_ack(9, 8))
				{
					i2c_read_Byte(9, 8, 0);
					i2c_stop(9, 8);
				}
				else cout << "ack erro 22" << "\n";
			}
			else cout << "ack erro23" << "\n";
		}
		else cout << "ack erro 24" << "\n";
	}

}




int main()
{
	
	wiringPiSetup();
	i2c_init(9, 8);
//	fd = wiringPiI2CSetup( 0x57 );
	pinMode( 15, INPUT);
	pullUpDnControl(15, PUD_UP);
	if( wiringPiISR( 15 , INT_EDGE_FALLING , &max30102_read_hr) < 0) cout << "20";

	max30102_init();
	while(1)
	{

	}

}
