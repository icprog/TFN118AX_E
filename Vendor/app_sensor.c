//app_sensor.c
#ifdef __TFN108

#include "stdint.h"
#include "app_init.h"
#include "stdbool.h"
#include "nrf_gpio.h"
#include "app_sensor.h"
#include "pah8011drv.h"
#define SSR_PWR_ON()										{nrf_gpio_cfg_output(SSR_PWREN);\
																					nrf_gpio_pin_set(SSR_PWREN);}//���ʿ���
#define SSR_PWR_OFF()										{nrf_gpio_pin_clear(SSR_PWREN);\
																					NRF_GPIO->PIN_CNF[SSR_PWREN] = GPIO_INVALID;}//���ʿ���

extern uint8_t RateErr;
extern uint8_t Off_Flag;
extern uint8_t Off_sec_Over;
uint8_t zero_flag;

uint16_t SampleOnTime;//��������ʱ��
uint32_t SampleTime;//�������ڣ���λ125ms
uint8_t SampleOnFlag = true;													//����������־
uint8_t SampleOverFlag;												//�������ڱ�־
uint32_t SampleCounter;												//�������ڼ�����,125ms
#if 0
uint8_t PulseArray[RATE_BUF_LEN];//����������
uint8_t IntervalArray[RATE_BUF_LEN];//����������
uint8_t PulseCount;//�����������
uint8_t IntervalCount;//�������ڸ���
uint16_t sum_interval;//����ƽ��ֵ�ĺ�

uint8_t test_pulse;//����������
uint8_t test_interval;//�������ڿ��

uint8_t nb_array[NB_LEN];//����ȡֵ����
uint8_t IntervalBuffer[RATE_BUF_LEN];//�����㷨���ȡֵ
#endif
extern pah8011Data_t pah8011Data;
uint8_t SensorValue = 0xff;										//����ֵ

void CloseHeartSample(void)
{
	pah8011_power_off();
}

#if 0
/*******************************************************
��������������ֵ�˲�����
��  ��:��
��  ��:��
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
	
	//ð������
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
��������������ʸ���
��  ��:��
��  ��:��
*******************************************************/
#define INTERFERENCE_LEVEL	6	//��λ10ms
uint8_t CheckInterference(void)
{
	uint8_t i,k=0,j=0,ret = false;
	for(i = 1;i<PulseCount;i++)
	{
		if(PulseArray[i] < INTERFERENCE_LEVEL)//����С��140ms
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
������������ҵ����
��  ��:��
��  ��:��
*******************************************************/
void Sensor_Deal(void)
{
	SampleOverFlag = 0;
//	if(Off_Flag)//��ⳬʱ�������ժ�������������ʼ��
//		return;
	if(SampleTime >= SAMPLE_MIN_TIME)
	{
		SampleCounter = SampleTime;						//׼����һ�ֲ���
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
���������������Э������
��  ��:��
��  ��:��
*******************************************************/
void OFFaboutRate(void)
{	
	if(Off_Flag)																		//���ժ��,�ر����ʼ��
	{
		SampleOnFlag = false;
		SampleOverFlag = 0;
		pah8011Data.close_flag = 1;
//		CloseHeartSample();
	}else{
		if(Off_sec_Over)																//�����������
		{
			Off_sec_Over = 0;
			if(Off_Flag == 0)															//������ϵ�ʱ��
				SampleOverFlag = 1;													//�������ʼ��
		}
	}
}

/*
��������������ʱ
*/
void Sensor_timeout(void)
{
	//��������ʱ��
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
����������ʱ���жϴ�����
��  ��:��
��  ��:��
��  �ܣ���������ʼ���к�ֹͣ���в�����10ms�������
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
//		//��������ʱ��
//		if(SampleOnTime)
//		{
//			SampleOnTime --;
//			if(SampleOnTime == 0)
//			{
//				selfon = false;
//				if(SampleTime >= SAMPLE_MIN_TIME)//10��,��SensorDeal����
//				{
//					SampleOnFlag = false;
////					CloseHeartSample();
//				}else
//				{
//					selfon = true;
//				}
//				if(IntervalCount)//����һ�ε�ֵ�����Чֵ
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
//					RateErr = 1;//�����쳣
//					if(SensorValue > HEARTRATE_MAX || SensorValue < HEARTRATE_MIN)
//					{
//						SensorValue = 0xff;
//					}
//				}
//				else if(zero_flag == 0)//������
//				{
//					SensorValue = 0x0;
//					RateErr = 1;//�����쳣
//				}
//				if(selfon)
//				{
//					SampleOnTime = SAMPLE_ON_TIME;			//8s��������
//					test_pulse = 0;
//					test_interval = 0;
//					PulseCount = 0;
//					IntervalCount = 0;
//				}
//			}
//		}
//		//����
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
//				if(current)//������
//				{
//					if(test_interval > 20)//�������>260ms,������С��230��/��
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
//				}//�½���
//				else
//				{
//					//��¼ʱ��
//					if(test_pulse>5)//�ߵ�ƽ����ʱ��>65ms
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
