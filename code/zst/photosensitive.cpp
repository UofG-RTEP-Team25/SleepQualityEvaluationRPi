#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>

#define		makerobo_PCF     120  // 基础管脚120

// 主函数
int main()
{
	int makerobo_analogVal;
	// 初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !");
		return 1;
	}
	// 在基本引脚120上设置pcf8591，地址0x48
	pcf8591Setup(makerobo_PCF, 0x48);

	while(1) // 无限循环
	{
		makerobo_analogVal = analogRead(makerobo_PCF + 0);       // 获取AIN0上的值,读取光敏传感器值
		printf("Photoresistor Value: %d\n", makerobo_analogVal); // 打印出光敏传感器的值

		delay (200);                                             // 延时200ms
	}
	return 0;
}
