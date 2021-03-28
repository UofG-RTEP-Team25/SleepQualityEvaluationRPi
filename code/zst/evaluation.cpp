#include <iostream>
using namespace std;

int main()
{
	int light_signal = 0;
	int sound_signal = 0;
	float c d e f;

	if( 检测到关灯 )
	{
		light_signal = 1;
	}

	if( 运动传感器十五分钟未移动 )
	{
		sound_signal = 1;
	}

	while( light_signal ==1 and sound_signal ==1)
	{
		if( heart_beat < 70)
		{
			c = 1;
		}
		else if( heart_beat >= 70 and heart_beat < 80)
		{
			c = 0.95;
		}
		else if( heart_beat >= 80 and heart_beat < 90)
		{
			c = 0.9;
		}
		else if( heart_beat >= 90 )
		{
			c = 0.8;
		}
	}

	while( light_signal ==1 and sound_signal ==1)
	{
		if( move_times < 5)
		{
			d = 1;
		}
		else if( move_times >= 5 and move_times < 10)
		{
			d = 0.95;
		}
		else if( move_times >= 10 and move_times < 15)
		{
			d = 0.9;
		}
		else if( move_times >= 15)
		{
			d = 0.8;
		}
	}

	while( light_signal ==1 and sound_signal ==1)
	{
		if( sounds )
		{
			e = 0.8;
		}
		else 
		{
			e = 1;
		}
	}


}
