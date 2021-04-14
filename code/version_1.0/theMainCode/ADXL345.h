#ifndef ADXL345_H_
#define ADXL345_H_



#define THRESH_TAP 0X1D
#define OFSX       0X1E
#define OFSY       0x1F
#define OFSZ       0x20
#define DUR        0x21
#define Latent     0x22
#define Window     0x23
#define THRESH_ACT 0x24
#define THRESH_INACT 0x25
#define TIME_INACT 0x26
#define ACT_INACT_CTL 0x27
#define THRESH_FF     0x28
#define TIME_FF       0x29
#define TAP_AXES      0x2A
#define ACT_TAP_STATUS 0x2B
#define BW_RATE        0x2C
#define POWER_CTL      0x2D
#define INT_ENABLE     0x2E
#define INT_MAP        0x2F
#define INT_SOURCE     0x30
#define DATA_FORMAT    0X31
#define DATAX0         0x32
#define DATAX1         0x33
#define DATAY0         0x34
#define DATAY1         0x35
#define DATAZ0         0x36
#define DATAZ1         0x37
#define FIFO_CTL       0x38
#define FIFO_STATUS    0x39

#define WRITE_ADDR     0XA6
#define READ_ADDR      0XA7

void move_detect();
void adxl345_init();



#endif

