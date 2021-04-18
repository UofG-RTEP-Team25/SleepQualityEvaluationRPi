#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <wiringPi.h>
#include <thread>
#include <sys/time.h>
#include <mysql/mysql.h>
#include "i2c.h"
#include "ADXL345.h"
#include "MAX30102.h"
#include "MAX44009.h"

using namespace std;

int32_t time_count = 0 , mov_count = 0 , sound_count = 0 , hr_count = 0 , t1 , t2;
int count = 0 , hr = 0 , sleep_status = 0;
double lux , pre_lux , efficiency , mark1 , mark2 , mark3;
int  Db = 0 , mov = 0 , flag = 1 , overall_mark;





void UploadData(int hr, double lux, int mov, int Db , int flag , int sleep_status)
{
    int HR = hr , appraise;
    float LUX = lux;
    char DB[20] = "quiet" , MOV[20] = "inactive";
    char time[20] = "0";

    switch(sleep_status)
    {
	    case 0: appraise = 101;
		    break;

            case 1: appraise = 101;
		    break;

	    case 2: appraise = 102;
		    break;

            case 3: appraise = overall_mark;
		    break;

    }

    if(Db == 0) strcpy(DB , "quiet");
    else strcpy(DB , "noisy");
    if(mov == 0) strcpy(MOV , "inactive");
    else strcpy(MOV , "active");
    
    #pragma endregion Prepare

    #pragma region Upload
    MYSQL *conn;
    int res;
    char str[185] , clearstr[185];
    conn = mysql_init( NULL );
    if(mysql_real_connect(conn, "78.141.235.21","example_user","password","sleepquality_db",0,NULL,CLIENT_FOUND_ROWS))
    {
        cout << "connect success" << endl;
        //res = mysql_query(conn, "insert into test values('user','123456')");

	if(flag == 1)
	{
		snprintf(clearstr, 185, "truncate table sleepquality_db.newinformation_list");
		flag = 0;
		mysql_query(conn , clearstr);
	}
	else
	{

		snprintf(str, 185, "insert into sleepquality_db.newinformation_list (hr, mov, DB, lux, time , appraise) values (%d, '%s', '%s', %f, '%s' , %d);", HR, MOV, DB, LUX, time , appraise);
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
	}
		mysql_close(conn);
    }
    else
    {
	    cout << "connect failed" << endl;
    }
    return;
    #pragma endregion Upload 


}



void timer_handler(int sig)
{
	time_count++;
}



void timer_enable()
{
	struct itimerval val;
	signal(SIGALRM , timer_handler);
	val.it_value.tv_sec = 1;
	val.it_value.tv_usec = 0;
	val.it_interval = val.it_value;
	setitimer(ITIMER_REAL , &val , NULL);

}

void timer_disable()
{
	struct itimerval val;
	val.it_value.tv_sec = 0;
	val.it_value.tv_usec = 0;
	val.it_interval = val.it_value;
	setitimer(ITIMER_REAL , &val , NULL);

}




void SleepEva()
{
	switch(sleep_status)
	{
		case 0:
			if(lux < 5)
			{
				if(time_count == 19) 
				{
					time_count = 0;
					sleep_status = 1;
					timer_enable();
				}
				else time_count++;
			}
			else 
			{
				time_count = 0;
			}
			break;

		case 1:
			if(hr > 45 && hr < 60 && mov == 0)
			{
				if(mov_count == 100)
				{
					t1 = time_count;
					time_count = 0;
					mov_count = 0;
					sleep_status = 2;

				}
				else mov_count++;
			}
			else 
			{
				mov_count = 0;
			}
			break;

		case 2:
			if(hr > 60 && hr < 110)
			{
				if(hr_count == 100)
				{
					t2 = time_count;
					timer_disable();
					time_count = 0;
					hr_count = 0;
					sleep_status = 3;
				}
				else hr_count++;
			}
			if(mov == 1) mov_count++;
			if(Db == 1) sound_count++;
			break;

		case 3:
			efficiency = (double)100*t2/(t1 + t2);

			if(t1 < 300) mark1 = 100;
			else if(t1 >= 300 && t1 < 600) mark1 = 80;
                        else if(t1 >= 600 && t1 < 900) mark1 = 60;
                        else if(t1 >= 900 && t1 < 1200) mark1 = 40;
                        else if(t1 >= 1200) mark1 = 0;


			if(efficiency >= 85 ) mark2 = 100;
                        else if(efficiency < 85 ) mark2 = 50;

			mark3 = 100*(mov_count + sound_count)/72000.00;

			overall_mark = 0.3*mark1 + 0.4*mark2 + 0.3*mark3;
			sleep_status = 0;
			break;

	}
}








int main(int argc,char *argv[])
{

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
		lux = max44009_read_light();

	//	cout << Db << endl;
	//	cout << lux << endl;
	//	cout << mov << endl;
	//	cout << hr << endl << endl;


		thread job2(SleepEva);
		thread job1(UploadData , hr , lux , mov , Db , flag , sleep_status);
		job2.join();
		job1.join();

		while(count)
		{
			count--;
			if(count == 0) Db = 0;
		}

		flag = 0;
		delay(3000);
	}



}
