#ifndef I2C_H_
#define I2C_H_


#define SDA_IN(x)  pinMode(x,INPUT)
#define SDA_OUT(x) pinMode(x,OUTPUT)



void i2c_init(char scl, char sda);
void i2c_start(char scl, char sda);
void i2c_stop(char scl, char sda);
void i2c_ack(char scl, char sda);
unsigned char i2c_wait_ack(char scl, char sda);
void i2c_nack(char scl, char sda);
unsigned char i2c_read_Byte(char scl, char sda, unsigned char data);
void i2c_send_Byte(char scl, char sda, int ack);


#endif
