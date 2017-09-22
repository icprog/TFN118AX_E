//app_sensor.c
#ifdef __TFN108

#include "stdint.h"
#include "app_init.h"
#include "stdbool.h"
#include "nrf_gpio.h"
#include "app_sensor.h"
#include "pah8011drv.h"
#define SSR_PWR_ON()										{nrf_gpio_cfg_output(SSR_PWREN);\
																					nrf_gpio_pin_set(SSR_PWREN);}//心率开关
#define SSR_PWR_OFF()										{nrf_gpio_pin_clear(SSR_PWREN);\
																					NRF_GPIO->PIN_CNF[SSR_PWREN] = GPIO_INVALID;}//心率开关

extern uint8_t RateErr;
extern uint8_t Off_Flag;
extern uint8_t Off_sec_Over;
uint8_t zero_flag;

uint16_t SampleOnTime;//采样持续时间
uint32_t SampleTime;//采样周期，单位125ms
uint8_t SampleOnFlag = true;													//采样启动标志
uint8_t SampleOverFlag;												//采样周期标志
uint32_t SampleCounter;												//采样周期计数器,125ms
#if 0
uint8_t PulseArray[RATE_BUF_LEN];//采样脉冲宽度
uint8_t IntervalArray[RATE_BUF_LEN];//采样间隔宽度
uint8_t PulseCount;//采样脉冲个数
uint8_t IntervalCount;//采样周期个数
uint16_t sum_interval;//计算平均值的和

uint8_t test_pulse;//单个脉冲宽度
uint8_t test_interval;//单个周期宽度

uint8_t nb_array[NB_LEN];//邻域取值缓冲
uint8_t IntervalBuffer[RATE_BUF_LEN];//邻域算法结果取值
#endif
extern pah8011Data_t pah8011Data;
uint8_t SensorValue = 0xff;										//传感值

void CloseHeartSample(void)
{
	pah8011_power_off();
}

#if 0
/*******************************************************
函数名：采用中值滤波处理
参  数:无
返  回:无
*******************************************************/
void MidValFilter(uint8_t *p,uint8_t idx,uint8_t len,uint8_t *pout)
{
	uint8_t i,j;
	int8_t tmp;
		
	for(i = 0;i < NB_LEN;i++)
	{
		tmp = i+idx-2;
		
		if(tmp < 0)
			tmp += len;
		if(tmp >= len)
			tmp -=len;
		
		nb_array[i] = p[tmp];
	}
	
	//冒泡排序
	for(i = 0;i<NB_LEN-1;i++)
	{
		for(j = 0;j < (NB_LEN - i -1);j++)
		{
			if(nb_array[j] > nb_array[j+1])
			{
				tmp=nb_array[j];
				nb_array[j] = nb_array[j+1];
				nb_array[j+1] = tmp;
			}				
		}
	}
	pout[idx] = nb_array[NB_LEN/2];
}
/*******************************************************
函数名：检测心率干扰
参  数:无
返  回:无
*******************************************************/
#define INTERFERENCE_LEVEL	6	//单位10ms
uint8_t CheckInterference(void)
{
	uint8_t i,k=0,j=0,ret = false;
	for(i = 1;i<PulseCount;i++)
	{
		if(PulseArray[i] < INTERFERENCE_LEVEL)//脉宽小于140ms
			k++;
	}
	
	if(k)
		ret = true;
	
	for(i = 1;i < IntervalCount;i++)
	{
		if(IntervalArray[i] < INTERFERENCE_LEVEL)
			j ++;
	}
	
	if(j)
		ret = true;
	
	return  ret;
}
#endif
/*******************************************************
函数名：心率业务处理
参  数:无
返  回:无
*******************************************************/
void Sensor_Deal(void)
{
	SampleOverFlag = 0;
//	if(Off_Flag)//检测超时，或腕带摘除，不开启心率检测
//		return;
	if(SampleTime >= SAMPLE_MIN_TIME)
	{
		SampleCounter = SampleTime;						//准备下一轮采样
	}
	SampleOnFlag = true;
	SampleOnTime = SAMPLE_ON_TIME;
	pah8011_power_on();
}

void Sensor_Value(void)
{
	uint8_t hrm = GetHeartValue();
	if(hrm)
	{
		SensorValue = hrm;
	}
}

/*******************************************************
函数名：心率佩戴协调机制
参  数:无
返  回:无
*******************************************************/
void OFFaboutRate(void)
{	
	if(Off_Flag)																		//腕带摘除,关闭心率检测
	{
		SampleOnFlag = false;
		SampleOverFlag = 0;
		pah8011Data.close_flag = 1;
//		CloseHeartSample();
	}else{
		if(Off_sec_Over)																//腕带佩戴后溢出
		{
			Off_sec_Over = 0;
			if(Off_Flag == 0)															//腕带带上的时刻
				SampleOverFlag = 1;													//触发心率检测
		}
	}
}

/*
函数名：采样超时
*/
void Sensor_timeout(void)
{
	//持续采样时间
	if(SampleOnTime)
	{
		SampleOnTime --;
		if(SampleOnTime == 0)
		{
			pah8011Data.close_flag = 1;
//			CloseHeartSample();
		}
	}
}
/*******************************************************
函数名：定时器中断处理函数
参  数:无
返  回:无
功  能：传感器开始运行和停止运行操作，10ms间隔唤醒
*******************************************************/
#if 0
//void TIMER1_IRQHandler(void)
//{
//	uint8_t i,selfon;
//	static uint8_t last,current;
//	if(NRF_TIMER1 ->EVENTS_COMPARE[0])
//	{
//		NRF_TIMER1 ->EVENTS_COMPARE[0] = 0;
//		NRF_TIMER1 ->TASKS_CLEAR = 1;
//		//持续采样时间
//		if(SampleOnTime)
//		{
//			SampleOnTime --;
//			if(SampleOnTime == 0)
//			{
//				selfon = false;
//				if(SampleTime >= SAMPLE_MIN_TIME)//10秒,由SensorDeal唤醒
//				{
//					SampleOnFlag = false;
////					CloseHeartSample();
//				}else
//				{
//					selfon = true;
//				}
//				if(IntervalCount)//用上一次的值填充无效值
//					IntervalArray[0] = SensorValue;
//				CheckInterference();
//				if((IntervalCount >= 3) && !CheckInterference())
//				{
//					for(i = 0;i < IntervalCount;i++)
//						MidValFilter(IntervalArray,i,IntervalCount,IntervalBuffer);
//					
//					sum_interval = 0;
//					for(i = 0;i<IntervalCount;i++)
//					{
//						sum_interval += IntervalBuffer[i];
//					}
//					SensorValue =  (uint32_t)IntervalCount *60000 /( sum_interval*13);
//					#ifdef __TEST
//					CheckInterference();
//					#endif
//					RateErr = 1;//心率异常
//					if(SensorValue > HEARTRATE_MAX || SensorValue < HEARTRATE_MIN)
//					{
//						SensorValue = 0xff;
//					}
//				}
//				else if(zero_flag == 0)//无心跳
//				{
//					SensorValue = 0x0;
//					RateErr = 1;//心率异常
//				}
//				if(selfon)
//				{
//					SampleOnTime = SAMPLE_ON_TIME;			//8s持续采样
//					test_pulse = 0;
//					test_interval = 0;
//					PulseCount = 0;
//					IntervalCount = 0;
//				}
//			}
//		}
//		//采样
//		if(SampleOnFlag)
//		{
//			nrf_gpio_cfg_input(SENSOR_IN,NRF_GPIO_PIN_PULLDOWN);
//			if(NRF_GPIO->IN & (1 << SENSOR_IN))
//			{
//				current = 1;
//			}else
//			{
//				current = 0;
//			}
//			NRF_GPIO->PIN_CNF[SENSOR_IN] = GPIO_INVALID;
//			test_pulse ++;
//			test_interval ++;
//			if(last != current)
//			{
//				test_pulse = 0;
//				zero_flag = 1;
//				if(current)//上升沿
//				{
//					if(test_interval > 20)//心跳间隔>260ms,即心跳小于230次/秒
//					{
//						if(IntervalCount < RATE_BUF_LEN)
//						{	
//							if(IntervalCount == 0)
//								IntervalArray[IntervalCount++] = 0xAA;
//							else
//								IntervalArray[IntervalCount++] = test_interval;
//							test_interval = 0;
//						}
//					}else{
//						test_pulse = 0;
//						test_interval = 0;
//					}
//				}//下降沿
//				else
//				{
//					//记录时间
//					if(test_pulse>5)//高电平持续时间>65ms
//					{
//						if(PulseCount < RATE_BUF_LEN)
//						{
//							if(PulseCount == 0)
//								PulseArray[PulseCount++] = 0xAA;
//							else
//								PulseArray[PulseCount++] = test_pulse;
//						}
//					}else{
//						test_pulse = 0;
//					}
//				}
//				last = current;
//			}
//		}
//	}
//}
#endif
#endif
//FILE END
