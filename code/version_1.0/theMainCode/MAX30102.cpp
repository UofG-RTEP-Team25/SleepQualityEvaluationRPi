#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include<wiringPi.h>
#include"MAX30102.h"
#include"algorithm.cpp"
#include"i2c.h"

using namespace std;

extern int hr;

int fd , rd_ptr = 0 , ir_data = 0 , tem , sample_num = 0;
uint32_t data_buffer[100] , non_buffer[100];
int32_t n_sp02 , n_heart_rate;
int8_t ch_spo2_valid , ch_hr_valid;
double temp;
unsigned char init_data_2[2] = { REG_INTR_STATUS_1 , REG_INTR_STATUS_2 };



void max30102_init()//initialize MAX30102 and set register configration.
{



        unsigned char reg_buffer[12] = { REG_MODE_CONFIG , REG_INTR_ENABLE_1 , REG_INTR_ENABLE_2 , REG_FIFO_WR_PTR , REG_OVF_COUNTER , REG_FIFO_RD_PTR , REG_FIFO_CONFIG , REG_MODE_CONFIG , REG_SPO2_CONFIG , REG_LED1_PA , REG_LED2_PA , REG_PILOT_LED } , init_data_1[12] = { 0x40 , 0x40 , 0x00 , 0x00 , 0x00 , 0x00 , 0x4f , 0x02 , 0x27 , 0x24 , 0x24 , 0x7f };
	int t, n;


	for(t = 0 ; t < 12; t++)
	{	
		i2c_write_reg8(9 , 8 , 0xAE , reg_buffer[t] , init_data_1[t]);

	}
	for(n = 0; n < 2; n++ )
	{
		i2c_read_reg8_singleByte(9 , 8 , 0xAE , 0xAF , init_data_2[n]);

	}


}


    


void max30102_read_hr( void )// read hear rates data from MAX30102.
{
	int n;

	//read and save data from FIFO.
	if(i2c_read_reg_Setup(9 , 8 , 0XAE , 0xAF , REG_FIFO_DATA))
	{
		tem = i2c_read_Byte(9, 8, 1) << 16;
		ir_data += tem;
		tem = i2c_read_Byte(9, 8, 1) << 8;
		ir_data += tem;
		tem = i2c_read_Byte(9, 8, 0);
		ir_data += tem;
		i2c_stop(9 , 8);

		ir_data &= 0x03ffff;
		data_buffer[sample_num] = ir_data;
	}

	//write FIFO read pointer register.
	i2c_write_reg8(9 , 8 , 0xAE , REG_FIFO_RD_PTR , rd_ptr);
	if(rd_ptr < 31) rd_ptr++;
	else rd_ptr = 0;

	//calculate heart rates every 100 samples.
	if(sample_num < 99) sample_num++;
	else	
	{
		sample_num = 0;
		maxim_heart_rate_and_oxygen_saturation( data_buffer , 100 , non_buffer , &n_sp02 , &ch_spo2_valid , &n_heart_rate , &ch_hr_valid);//calculate HR with saved data from FIFO.
		hr = n_heart_rate;
		
		//read chip temperature.
	/*	i2c_write_reg8(9 , 8 , 0xAE , 0x21 , 0x01);// enable temperature sensor
		temp = i2c_read_reg8_singleByte(9 , 8 , 0xAE , 0xAF , 0x1f) + 0.625*i2c_read_reg8_singleByte(9 , 8 , 0xAE , 0xAF , 0x20);//read data from register and calculate temperature.
		cout << "TEMP:" << temp << "\n" << "\n";*/
	}

	//clear interrupt.
	for(n = 0; n < 2; n++)
	{
		i2c_read_reg8_singleByte(9 , 8 , 0xAE , 0xAF , init_data_2[n]);
	}


}





