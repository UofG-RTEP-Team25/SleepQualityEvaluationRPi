#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mysql/mysql.h>
using namespace std;

int main(int argc,char *argv[])
{
    #pragma region Prepare
    int appraise =  80;
    int sound = 0;
    int light = 1;
    float m_x = 0.1f;
    float m_y = 0.2f;
    float m_z = 0.1f;
    char time[20] = "2021-03-20 02:59:30";
    char str[80];
    #pragma endregion Prepare

    #pragma region Upload
    MYSQL *conn;
    int res;
    conn = mysql_init( NULL );
    if(mysql_real_connect(conn, "78.141.235.21","example_user","password","sleepquality_db",0,NULL,CLIENT_FOUND_ROWS))
    {
        cout << "connect success" << endl;
        //res = mysql_query(conn, "insert into test values('user','123456')");

        sprintf(str,"insert * into sleepquality_db.information_list (appraise, sound, light, movement_x, movement_y, movement_z, time) values ('appraise=%d, sound=%d, light=%d, movement_x=%f, movement_y=%f, movement_z=%f, time=%s')", appraise, sound, light, m_x, m_y, m_z, time);

        if(mysql_query(conn,str))
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