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
unsigned char i2c_read_Byte(char scl, char sda, int ack);
void i2c_send_Byte(char scl, char sda, int data);
void i2c_write_reg8( char scl , char sda , unsigned char write_addr , unsigned char write_reg , int data);
unsigned char i2c_read_reg8_singleByte(char scl , char sda , unsigned char write_addr , unsigned char read_addr , unsigned char read_reg);
unsigned char  i2c_read_reg_Setup( char scl , char sda , unsigned char write_addr , unsigned char read_addr , unsigned char read_reg);
void i2c_write_Byte(char scl , char sda , unsigned char write_addr , int data);
int i2c_read( char scl , char sda , unsigned char read_addr , int ack);



#endif
