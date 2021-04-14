#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <wiringPi.h>
//#include <mysql/mysql.h>
#include "i2c.h"
#include "ADXL345.h"
#include "MAX30102.h"
#include "MAX44009.h"

using namespace std;

int main(int argc,char *argv[])
{
    #pragma region Prepare
    /***
     * 
     * Comment Writter:    Yangbolun Zhou
     * Date:               2021/03/20
     * 
     * appraise            top 100 minue 0 for evaluate if user have a good sleep
     * sound               int value for evaluate environment sound
     * light               int value for evaluate environment light
     * m_x                 float value for evaluate user's movement in x-axis
     * m_y                 float value for evaluate user's movement in y-axis
     * m_z                 float value for evaluate user's movement in z-axis
     * time                string value for record data's time, the format should be exactly like "YYYY-MM-DD hh:mm:ss"
     * 
     * All the value showing below is just an example, you can give your value read from sensor to them.
     * Do not forget to format to correct data type.
     * 
     * This script is using mysql.h so it is nesseary to install mariadb(mysql) first, I recommand to install both client and dev packages.
     * before to compile this file, it is nesseary to use g++ upload.cpp -o XXXX -I/usr/include/mysql -lmysqlclient
     * upload.cpp is this script or your can include this file and compile your main file by give your main file's name.
     * XXX is the output file's name
     * 
     **/





 /*****   int appraise =  95;
    int sound = 5;
    int light = 5;
    float m_x = 1.2f;
    float m_y = 1.1f;
    float m_z = 2.5f;
    char time[20] = "2021-03-20 04:06:35";
    
    #pragma endregion Prepare

    #pragma region Upload
    MYSQL *conn;
    int res;
    char str[185];
    conn = mysql_init( NULL );
    if(mysql_real_connect(conn, "78.141.235.21","example_user","password","sleepquality_db",0,NULL,CLIENT_FOUND_ROWS))
    {
        cout << "connect success" << endl;
        //res = mysql_query(conn, "insert into test values('user','123456')");

        snprintf(str, 185, "insert into sleepquality_db.information_list (appraise, sound, light, movement_x, movement_y, movement_z, time) values (%d, %d, %d, %f, %f, %f, '%s');", appraise, sound, light, m_x, m_y, m_z, time);
        cout << str << endl;
        //mysql_query(conn,str);
 
        if( !(mysql_query(conn,str)) )
        {
            printf("INSERT OK\n");
        }
        else
        {
            printf("INSERT ERROR\n");
        }
        mysql_close(conn);
    }else
    {
        cout << "connect failed" << endl;
    }
    return 0;
    #pragma endregion Upload *****/


	wiringPiSetup();
	i2c_init(12 , 13);//  I2C interface initialization for ADXL345
	i2c_init(9 , 8); //  I2C interface initialization for MAX30132
	i2c_init(21 , 22);// I2C interface initlization for MAX44009

	pinMode(30 , INPUT);
	pinMode(15 , INPUT);
	pinMode(1 , INPUT);

	pullUpDnControl(30 , PUD_DOWN);
	pullUpDnControl(15 , PUD_UP);
	pullUpDnControl(1 , PUD_UP);

	wiringPiISR(30 , INT_EDGE_RISING , &move_detect);
	wiringPiISR(15 , INT_EDGE_FALLING , &max30102_read_hr);
	wiringPiISR(1 , INT_EDGE_FALLING , &sound_detect);

	adxl345_init();
	max44009_init();
	max30102_init();


	while(1)
	{
		max44009_read_light();
		delay(2000);
	}



}
