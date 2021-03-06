#ifndef MAX30102_H_
#define MAX30102_H_

#include<wiringPiI2C.h> 

//REGISTER ADDRESSES
#define REG_INTR_STATUS_1 0X00
#define REG_INTR_STATUS_2 0X01
#define REG_INTR_ENABLE_1 0X02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0X04
#define REG_OVF_COUNTER 0X05
#define REG_FIFO_RD_PTR 0X06
#define REG_FIFO_DATA 0X07
#define REG_FIFO_CONFIG 0X08
#define REG_MODE_CONFIG 0X09
#define REG_SPO2_CONFIG 0X0A
#define REG_LED1_PA 0X0C
#define REG_LED2_PA 0X0D
#define REG_PILOT_LED 0X10
#define REG_MULI_LED_CTRL1 0X11
#define REG_MUTI_LED_CTRL2 0X12
#define REG_TINT 0X1F
#define REG_TFRAC 0X20
#define REG_TEMP_CONFIG 0X21
#define REG_REV_ID 0XFE
#define REG_PART_ID 0XFF


void max30102_init();
void max30102_read_hr(void);




#endif


