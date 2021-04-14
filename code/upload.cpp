#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mysql/mysql.h>
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
     * m_x                 float value for evaluate user's movement
     * m_y                 deleted
     * m_z                 deleted
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

    int appraise =  95;
    int sound = 5;
    int light = 5;
    float m_x = 1.2f;
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

        snprintf(str, 185, "insert into sleepquality_db.information_list (appraise, sound, light, movement_x, movement_y, movement_z, time) values (%d, %d, %d, %f, %f, %f, '%s');", appraise, sound, light, m_x, 0.0f, 0.0f, time);
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
    #pragma endregion Upload
}