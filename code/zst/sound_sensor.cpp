#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>

#define makerobo_PCF       120  

int main (void)
{
	int sound_value;    
	wiringPiSetup ();   
	pcf8591Setup (makerobo_PCF, 0x48);

	while(1) 
	{
		sound_value = analogRead  (makerobo_PCF + 0); 
		printf("soundValue: %d\n", sound_value);  
		delay(200);             
	}
	return 0;
}
