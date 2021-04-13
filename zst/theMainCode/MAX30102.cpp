#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include<wiringPi.h>
#include<wiringPiI2C.h>
#include"MAX30102.h"
#include"algorithm.h"
#include <pcf8591.h>
#include <stdio.h>
#include "ADXL345.cpp"
using namespace std;

#define Address 0x48
#define BASE 0x40
#define A0 BASE+0
#define A1 BASE+1
ADXL  adx;
struct ADXL::acc_dat myadx_data;
int hr_hander_srate = 0;


int fd , rd_ptr = 0 , ir_data = 0 , tem , sample_num = 0;
uint32_t data_buffer[100] , non_buffer[100];
int32_t n_sp02 , n_heart_rate;
int8_t ch_spo2_valid , ch_hr_valid;
double temp;

void max30102_init()
{
	if( wiringPiI2CWriteReg8( fd , REG_MODE_CONFIG , 0X40) < 0) cout << "1";
	if( wiringPiI2CWriteReg8( fd , REG_INTR_ENABLE_1 , 0X40) < 0)  cout << "2";

	if( wiringPiI2CWriteReg8( fd , REG_INTR_ENABLE_2 , 0X00) < 0)   cout << "3";
	if( wiringPiI2CWriteReg8( fd , REG_FIFO_WR_PTR , 0x00) < 0)	cout << "4";
	if( wiringPiI2CWriteReg8( fd , REG_OVF_COUNTER , 0x00) < 0)	cout << "5";
	if( wiringPiI2CWriteReg8( fd , REG_FIFO_RD_PTR , 0x00) < 0)	cout << "6";
	if( wiringPiI2CWriteReg8( fd , REG_FIFO_CONFIG , 0x4f) < 0)	cout << "7";
	if( wiringPiI2CWriteReg8( fd , REG_MODE_CONFIG , 0X02) < 0)	cout << "8";
	if( wiringPiI2CWriteReg8( fd , REG_SPO2_CONFIG , 0X27) < 0)	cout << "9";
	if( wiringPiI2CWriteReg8( fd , REG_LED1_PA , 0X24) < 0) 	cout << "10";
	if( wiringPiI2CWriteReg8( fd , REG_LED2_PA , 0X24) < 0) 	cout << "11";
	if( wiringPiI2CWriteReg8( fd , REG_PILOT_LED , 0X7F) < 0)	cout << "11";
	if( wiringPiI2CReadReg8( fd, REG_INTR_STATUS_1) < 0)		cout << "12";
	if( wiringPiI2CReadReg8( fd, REG_INTR_STATUS_2) < 0)	cout << "13";

}

void maxim_heart_rate_and_oxygen_saturation(uint32_t *pun_ir_buffer,  int32_t n_ir_buffer_length, uint32_t *pun_red_buffer, int32_t *pn_spo2, int8_t *pch_spo2_valid, 
                              int32_t *pn_heart_rate, int8_t  *pch_hr_valid)
{
    uint32_t un_ir_mean ,un_only_once ;
    int32_t k ,n_i_ratio_count;
    int32_t i, s, m, n_exact_ir_valley_locs_count ,n_middle_idx;
    int32_t n_th1, n_npks,n_c_min;      
    int32_t an_ir_valley_locs[15] ;
    int32_t an_exact_ir_valley_locs[15] ;
    int32_t an_dx_peak_locs[15] ;
    int32_t n_peak_interval_sum;
    
    int32_t n_y_ac, n_x_ac;
    int32_t n_spo2_calc; 
    int32_t n_y_dc_max, n_x_dc_max; 
    int32_t n_y_dc_max_idx, n_x_dc_max_idx; 
    int32_t an_ratio[5],n_ratio_average; 
    int32_t n_nume,  n_denom ;

    un_ir_mean =0; 
    for (k=0 ; k<n_ir_buffer_length ; k++ ) un_ir_mean += pun_ir_buffer[k] ;
    un_ir_mean =un_ir_mean/n_ir_buffer_length ;
    for (k=0 ; k<n_ir_buffer_length ; k++ )  an_x[k] =  pun_ir_buffer[k] - un_ir_mean ;


    for(k=0; k< BUFFER_SIZE-MA4_SIZE; k++){
        n_denom= ( an_x[k]+an_x[k+1]+ an_x[k+2]+ an_x[k+3]);
        an_x[k]=  n_denom/(int32_t)4; 
    }


    for( k=0; k<BUFFER_SIZE-MA4_SIZE-1;  k++)
        an_dx[k]= (an_x[k+1]- an_x[k]);

    for(k=0; k< BUFFER_SIZE-MA4_SIZE-2; k++){
        an_dx[k] =  ( an_dx[k]+an_dx[k+1])/2 ;
    }
    
    for ( i=0 ; i<BUFFER_SIZE-HAMMING_SIZE-MA4_SIZE-2 ;i++){
        s= 0;
        for( k=i; k<i+ HAMMING_SIZE ;k++){
            s -= an_dx[k] *auw_hamm[k-i] ; 
                     }
        an_dx[i]= s/ (int32_t)1146; // divide by sum of auw_hamm 
    }

    n_th1=0; // threshold calculation
    for ( k=0 ; k<BUFFER_SIZE-HAMMING_SIZE ;k++){
        n_th1 += ((an_dx[k]>0)? an_dx[k] : ((int32_t)0-an_dx[k])) ;
    }

    n_th1= n_th1/ ( BUFFER_SIZE-HAMMING_SIZE);
    maxim_find_peaks( an_dx_peak_locs, &n_npks, an_dx, BUFFER_SIZE-HAMMING_SIZE, n_th1, 8, 15 );//peak_height, peak_distance, max_num_peaks 
    n_peak_interval_sum =0;
    if (n_npks>=2){
        for (k=1; k<n_npks; k++)
            n_peak_interval_sum += (an_dx_peak_locs[k]-an_dx_peak_locs[k -1]);
        n_peak_interval_sum=n_peak_interval_sum/(n_npks-1);
        *pn_heart_rate=(int32_t)(1500/n_peak_interval_sum);// beats per minutes
        *pch_hr_valid  = 1;
    }
    else  {
        *pn_heart_rate = -999;
        *pch_hr_valid  = 0;
    }
            
    for ( k=0 ; k<n_npks ;k++)
        an_ir_valley_locs[k]=an_dx_peak_locs[k]+HAMMING_SIZE/2; 


    // raw value : RED(=y) and IR(=X)
    // we need to assess DC and AC value of ir and red PPG. 
    for (k=0 ; k<n_ir_buffer_length ; k++ )  {
        an_x[k] =  pun_ir_buffer[k] ; 
        an_y[k] =  pun_red_buffer[k] ; 
    }

    // find precise min near an_ir_valley_locs
    n_exact_ir_valley_locs_count =0; 
    for(k=0 ; k<n_npks ;k++){
        un_only_once =1;
        m=an_ir_valley_locs[k];
        n_c_min= 16777216;//2^24;
        if (m+5 <  BUFFER_SIZE-HAMMING_SIZE  && m-5 >0){
            for(i= m-5;i<m+5; i++)
                if (an_x[i]<n_c_min){
                    if (un_only_once >0){
                       un_only_once =0;
                   } 
                   n_c_min= an_x[i] ;
                   an_exact_ir_valley_locs[k]=i;
                }
            if (un_only_once ==0)
                n_exact_ir_valley_locs_count ++ ;
        }
    }
    if (n_exact_ir_valley_locs_count <2 ){
       *pn_spo2 =  -999 ; // do not use SPO2 since signal ratio is out of range
       *pch_spo2_valid  = 0; 
       return;
    }


   for(k=0; k< BUFFER_SIZE-MA4_SIZE; k++){
        an_x[k]=( an_x[k]+an_x[k+1]+ an_x[k+2]+ an_x[k+3])/(int32_t)4;
        an_y[k]=( an_y[k]+an_y[k+1]+ an_y[k+2]+ an_y[k+3])/(int32_t)4;
    }

    //using an_exact_ir_valley_locs , find ir-red DC andir-red AC for SPO2 calibration ratio
    //finding AC/DC maximum of raw ir * red between two valley locations
    n_ratio_average =0; 
    n_i_ratio_count =0; 
    
    for(k=0; k< 5; k++) an_ratio[k]=0;
    for (k=0; k< n_exact_ir_valley_locs_count; k++){
        if (an_exact_ir_valley_locs[k] > BUFFER_SIZE ){             
            *pn_spo2 =  -999 ; // do not use SPO2 since valley loc is out of range
            *pch_spo2_valid  = 0; 
            return;
        }
    }
    // find max between two valley locations 
    // and use ratio betwen AC compoent of Ir & Red and DC compoent of Ir & Red for SPO2 

    for (k=0; k< n_exact_ir_valley_locs_count-1; k++){
        n_y_dc_max= -16777216 ; 
        n_x_dc_max= - 16777216; 
        if (an_exact_ir_valley_locs[k+1]-an_exact_ir_valley_locs[k] >10){
            for (i=an_exact_ir_valley_locs[k]; i< an_exact_ir_valley_locs[k+1]; i++){
                if (an_x[i]> n_x_dc_max) {n_x_dc_max =an_x[i];n_x_dc_max_idx =i; }
                if (an_y[i]> n_y_dc_max) {n_y_dc_max =an_y[i];n_y_dc_max_idx=i;}
            }
            n_y_ac= (an_y[an_exact_ir_valley_locs[k+1]] - an_y[an_exact_ir_valley_locs[k] ] )*(n_y_dc_max_idx -an_exact_ir_valley_locs[k]); //red
            n_y_ac=  an_y[an_exact_ir_valley_locs[k]] + n_y_ac/ (an_exact_ir_valley_locs[k+1] - an_exact_ir_valley_locs[k])  ; 
        
        
            n_y_ac=  an_y[n_y_dc_max_idx] - n_y_ac;    // subracting linear DC compoenents from raw 
            n_x_ac= (an_x[an_exact_ir_valley_locs[k+1]] - an_x[an_exact_ir_valley_locs[k] ] )*(n_x_dc_max_idx -an_exact_ir_valley_locs[k]); // ir
            n_x_ac=  an_x[an_exact_ir_valley_locs[k]] + n_x_ac/ (an_exact_ir_valley_locs[k+1] - an_exact_ir_valley_locs[k]); 
            n_x_ac=  an_x[n_y_dc_max_idx] - n_x_ac;      // subracting linear DC compoenents from raw 
            n_nume=( n_y_ac *n_x_dc_max)>>7 ; //prepare X100 to preserve floating value
            n_denom= ( n_x_ac *n_y_dc_max)>>7;
            if (n_denom>0  && n_i_ratio_count <5 &&  n_nume != 0)
            {   
                an_ratio[n_i_ratio_count]= (n_nume*100)/n_denom ; //formular is ( n_y_ac *n_x_dc_max) / ( n_x_ac *n_y_dc_max) ;
                n_i_ratio_count++;
            }
        }
    }

    maxim_sort_ascend(an_ratio, n_i_ratio_count);
    n_middle_idx= n_i_ratio_count/2;

   if (n_middle_idx >1)
        n_ratio_average =( an_ratio[n_middle_idx-1] +an_ratio[n_middle_idx])/2; // use median
    else
        n_ratio_average = an_ratio[n_middle_idx ];

    if( n_ratio_average>2 && n_ratio_average <184){
        n_spo2_calc= uch_spo2_table[n_ratio_average] ;
        *pn_spo2 = n_spo2_calc ;
        *pch_spo2_valid  = 1;//  float_SPO2 =  -45.060*n_ratio_average* n_ratio_average/10000 + 30.354 *n_ratio_average/100 + 94.845 ;  // for comparison with table
    }
    else{
        *pn_spo2 =  -999 ; // do not use SPO2 since signal ratio is out of range
        *pch_spo2_valid  = 0; 
    }
}

void maxim_find_peaks(int32_t *pn_locs, int32_t *pn_npks, int32_t *pn_x, int32_t n_size, int32_t n_min_height, int32_t n_min_distance, int32_t n_max_num)
{
    maxim_peaks_above_min_height( pn_locs, pn_npks, pn_x, n_size, n_min_height );
    maxim_remove_close_peaks( pn_locs, pn_npks, pn_x, n_min_distance );
    *pn_npks = min( *pn_npks, n_max_num );
}

void maxim_peaks_above_min_height(int32_t *pn_locs, int32_t *pn_npks, int32_t  *pn_x, int32_t n_size, int32_t n_min_height)
{
    int32_t i = 1, n_width;
    *pn_npks = 0;
    
    while (i < n_size-1){
        if (pn_x[i] > n_min_height && pn_x[i] > pn_x[i-1]){            // find left edge of potential peaks
            n_width = 1;
            while (i+n_width < n_size && pn_x[i] == pn_x[i+n_width])    // find flat peaks
                n_width++;
            if (pn_x[i] > pn_x[i+n_width] && (*pn_npks) < 15 ){                            // find right edge of peaks
                pn_locs[(*pn_npks)++] = i;        
                // for flat peaks, peak location is left edge
                i += n_width+1;
            }
            else
                i += n_width;
        }
        else
            i++;
    }
}

void maxim_remove_close_peaks(int32_t *pn_locs, int32_t *pn_npks, int32_t *pn_x, int32_t n_min_distance)
{
    
    int32_t i, j, n_old_npks, n_dist;
    
    /* Order peaks from large to small */
    maxim_sort_indices_descend( pn_x, pn_locs, *pn_npks );

    for ( i = -1; i < *pn_npks; i++ ){
        n_old_npks = *pn_npks;
        *pn_npks = i+1;
        for ( j = i+1; j < n_old_npks; j++ ){
            n_dist =  pn_locs[j] - ( i == -1 ? -1 : pn_locs[i] ); // lag-zero peak of autocorr is at index -1
            if ( n_dist > n_min_distance || n_dist < -n_min_distance )
                pn_locs[(*pn_npks)++] = pn_locs[j];
        }
    }

    // Resort indices longo ascending order
    maxim_sort_ascend( pn_locs, *pn_npks );
}

void maxim_sort_ascend(int32_t *pn_x,int32_t n_size) 
{
    int32_t i, j, n_temp;
    for (i = 1; i < n_size; i++) {
        n_temp = pn_x[i];
        for (j = i; j > 0 && n_temp < pn_x[j-1]; j--)
            pn_x[j] = pn_x[j-1];
        pn_x[j] = n_temp;
    }
}

void maxim_sort_indices_descend(int32_t *pn_x, int32_t *pn_indx, int32_t n_size)
{
    int32_t i, j, n_temp;
    for (i = 1; i < n_size; i++) {
        n_temp = pn_indx[i];
        for (j = i; j > 0 && pn_x[n_temp] > pn_x[pn_indx[j-1]]; j--)
            pn_indx[j] = pn_indx[j-1];
        pn_indx[j] = n_temp;
    }
}

void max30102_read_hr( void )
{
	wiringPiI2CWrite( fd , REG_FIFO_DATA);
	tem = wiringPiI2CRead( fd ) << 16;
	ir_data += tem;
	tem = wiringPiI2CRead( fd ) << 8;
	ir_data += tem;
	tem = wiringPiI2CRead( fd );
	ir_data += tem;
	ir_data &= 0x03ffff;
	data_buffer[sample_num] = ir_data;

	if( wiringPiI2CWriteReg8( fd , REG_FIFO_RD_PTR , rd_ptr ) < 0) cout << "21";
	else if( rd_ptr < 31) ++rd_ptr;  else rd_ptr = 0;

	if(sample_num < 99) sample_num++;
	else	
	{
		sample_num = 0;
		maxim_heart_rate_and_oxygen_saturation( data_buffer , 100 , non_buffer , &n_sp02 , &ch_spo2_valid , &n_heart_rate , &ch_hr_valid);
		hr_hander_srate=1;
	}

	if( wiringPiI2CReadReg8( fd, REG_INTR_STATUS_1) < 0) cout << "14";
	if( wiringPiI2CReadReg8( fd, REG_INTR_STATUS_2) < 0)	cout << "15";


}




int  light(){    //返回光强度  
	return analogRead(A1);
}
int  sound(){    //返回声音强度 电位器可调灵敏度
	return analogRead(A0);
}
char adx_state_last[3]={0};  //之前的状态
char adx_state_now[3]={0};   //现在的状态
int ADX_move(){ //返回三轴状态 简单判断是否倾斜
	int a=99,b=99,c=99;
	myadx_data = adx.adxl345_read_xyz();
	//printf("x: %05d  y: %05d  z: %05d\n", myadx_data.x, myadx_data.y, myadx_data.z);
	if(myadx_data.x<10000){a=1;}else{a=0;}
	if(myadx_data.y<10000){b=1;}else{b=0;}
	if(myadx_data.z<10000){c=1;}else{c=0;}
	adx_state_now[0] = a;adx_state_now[1] = b;adx_state_now[2] = c;//当前状态
	//比较
	if(adx_state_now[0]!=adx_state_last[0]||adx_state_now[1]!=adx_state_last[1]||adx_state_now[2]!=adx_state_last[2]){
		adx_state_last[0] = adx_state_now[0];adx_state_last[1] = adx_state_now[1];adx_state_last[2] = adx_state_now[2]; 
		return 0;//状态已经改变
	}else{
		adx_state_last[0] = adx_state_now[0];adx_state_last[1] = adx_state_now[1];adx_state_last[2] = adx_state_now[2]; 
		return 1;//状态没有改变
	}
	
}

float hr_dispose(int32_t data){
	if(data<70){return 1.0;}
	if(data>70&&data<80){return 0.95;}
	if(data>80&&data<90){return 0.9;}
	if(data>90){return 0.8;}
	return 99.99;
}
float sound_dispose(int data){
	if(data>120){return 0.8;}else{return 1;}
	return 99.99;
}
float light_dispose(int data){
	if(data==0){return 1.0;}
	if(data==1){return 0.95;}
	if(data==2){return 0.9;}
	if(data>2){return 0.8;}
	return 99.99;
}
float ADX_dispose(int data){
	if(data<5){return 1.0;}
	if(data>5&&data<10){return 0.95;}
	if(data>10&&data<15){return 0.9;}
	if(data>15){return 0.8;}
	return 99.99;
}

int state_adx=99;
int sp_state = 0;//程序运行位置逻辑标志位
int time_num = 0; //时间计数
int push = 0;
int push_time = 0; 
int last_time = 0;

int hr_num=0;
int sound_num=0;
int light_num=0;
int adx_num=0;

float c,d,e,f; 
float result;
void start()   //逻辑开始
{
	if(light()>120)  //变暗开始睡觉了 但是没睡着 该值可根据具体环境调整
	{
	printf("!!!!!!!!!!!start!!!!!!!\r\n");		
	sp_state=1;
	/*while(sp_state==1)  
	{
		if(time_num>=1) //1分钟取一次状态
		{
			if(push!=0){
				state_adx=ADX_move(); push=1; 
				time_num=0;
				printf(" state_adx:%d\r\n", state_adx);
			}
			if(push==0){
				state_adx=ADX_move(); push=1; //第一次的值抛弃 因为第一次是为了初始化两个状态数组
				time_num=0;
				state_adx=99;
			}
			last_time++;
		}
		if(state_adx==0)  //状态改变 退出循环 重新开始
		{
			sp_state=-1;  //改变SP 退出循环
		}
		if(analogRead(A1)<120) {  //中途开灯 取消评估 从头开始
			sp_state=-1;  //改变SP 退出循环
		}
		if(push_time>=120){		//1分钟到  
			time_num++;
			push_time=0;
		}
		delay(500); 	//如果想调试快一点看结果 时间改短即可 比如5
		push_time++;
		if(last_time>=15){  //十五分钟不移动就进入评估状态 进入评估状态程序不会重新开始了 只能手动取消运行重新开始
			sp_state=2;
			last_time=0;
		}
	}*/
	sp_state=2;
	while(sp_state==2){   //睡眠质量评估
			if(last_time>=100)  //5s到
			{
				printf("hr=%d ADX=%d sound=%d light=%d\r\n",hr_num,adx_num,light_num,sound());
				c=hr_dispose(hr_num);  	//心跳
				d=ADX_dispose(adx_num);			//移动
				e=sound_dispose(sound());		//声音更新
				f=light_dispose(light_num);		//开关灯次数更新
				result = c*d*e*f;				//计算
				printf("c:%f d:%f e:%f f:%f",c,d,e,f);
				printf("result:%f\r\n",result);
				last_time = 0;
				light_num = 0;
				adx_num=0;
			}
			if(light()<120){      //开了灯
				light_num++;
			}
			state_adx=ADX_move();
			if(state_adx==0){  //移动了
				adx_num++;
			}
			delay(50);
			last_time++;
			if(hr_hander_srate==1){   //心跳中断
				hr_num = n_heart_rate;
				hr_hander_srate=0;
			}
		}
	}
}




//初始化
 /*-----------------主体-----------------*/
 int setup(){    
	if(wiringPiSetup()==-1)return 1;
	fd = wiringPiI2CSetup( 0x57 );
	pinMode( 15, INPUT);
	pullUpDnControl(15, PUD_UP);
	if( wiringPiISR( 15 , INT_EDGE_FALLING , &max30102_read_hr) < 0) cout << "20";
    pcf8591Setup(BASE, Address);
	max30102_init();
	adx.adxl345_init();
     return 0;
 }


//主函数 
 int main(void){
     setup();
        while(1)
        {
            start();   //逻辑开始
        }
     return 0;
}

