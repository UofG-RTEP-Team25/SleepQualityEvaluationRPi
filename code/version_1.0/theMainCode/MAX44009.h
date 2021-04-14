#ifndef MAX44009_H_
#define MAX44009_H_





#define INTR_STATUS  0X00
#define INTR_ENABLE  0X01
#define REG_CONFIG   0X02
#define REG_LUX_HIGH 0x03
#define REG_LUX_LOW  0x04
#define THRESH_UP    0X05
#define THRESH_LOW   0x06
#define THRESH_TIME  0x07


#define WRITE_ADDR_GND   0X94
#define READ_ADDR_GND    0X95
#define WRITE_ADDR_VCC   0x96
#define READ_ADDR_VCC    0X97



void max44009_init();
void max44009_read_light();
void sound_detect();




#endif
