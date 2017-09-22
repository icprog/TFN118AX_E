//app_rtc.c
#include "nrf.h"
#include "stdint.h"
#include "stdbool.h"
#include "app_init.h"
#include "nrf_gpio.h"
#include "pah8011drv.h"
extern uint32_t DayCounter;								//ÌìÃëÊý
extern uint8_t DayOverFlag;
extern uint8_t Key;
extern uint8_t led_flag;
#ifdef __TFN108
extern uint32_t SampleCounter;
extern uint8_t SampleOverFlag;
extern uint8_t SensorValue;
extern uint8_t SampleOnFlag;
#endif
extern uint8_t CycleSendFlag;
extern uint8_t Off_sec_Delay;							//Íó´øÕª³ýÑÓÊ±
extern uint8_t Off_sec_Over;
extern uint8_t Shake;
extern uint8_t RateErr;
extern uint8_t Off_Flag;

extern pah8011Data_t pah8011Data;
/*
Íó´øÕª³ý¼ì²â
*/
#ifdef __TFN108
void OFF_Deal(void)
{
	static uint8_t off_count,on_count;	
	if(!nrf_gpio_pin_read(IO_OFF_I))							 		//Íó´øÕª³ý
	{
		if(Off_Flag)
			on_count = 0;
		else{
			if(off_count > 24){
				if(!Off_Flag)
				{
					Shake = 1;
					RateErr = 1;
					Off_Flag = 1;															//Õª³ý±êÖ¾
					SensorValue = 0xff;
				}
			}else{
				off_count++;
			}
		}
	}else{																						//ºÏÉÏ
		if(!Off_Flag)
			off_count = 0;
		else{
			if(on_count > 16){
				if(Off_Flag)																//Õª³ýÇÐ»»µ½ÅäÅå´ø
				{
					Off_sec_Delay = OFF_SEC_DELAY;
					Off_sec_Over = 0;
					SampleOverFlag = 0;
					Off_Flag = 0;
				}
			}else{
				on_count++;
			}
		}
	}
//	nrf_gpio_cfg_input(IO_OFF_I,NRF_GPIO_PIN_PULLDOWN);						//¹Ø±Õ¼ì²â
}
#endif
void RTC0_IRQHandler(void)
{
	static uint8_t num;
	static uint8_t i;
	if(NRF_RTC0->EVENTS_TICK)//
	{
		NRF_RTC0->EVENTS_TICK = 0;
		NRF_RTC0->TASKS_CLEAR = 1;
		
		nrf_gpio_cfg_input(IO_OFF_I,NRF_GPIO_PIN_NOPULL);
		i++;
		if(i==8)
		{
			i = 0;
			CycleSendFlag = 1;//ÉäÆµ·¢ËÍ
			if(DayCounter--)
			{
				if(DayCounter== 0)
				{
					DayOverFlag = 1;
				}
			}
			//°´¼ü¼ì²â
			nrf_gpio_cfg_input(KEY,NRF_GPIO_PIN_PULLUP);
			if(!nrf_gpio_pin_read(KEY))
			{
				if(num < 3)
				{
					num++;
					if(num == 3)
					{	
						Key = 1;
						led_flag = 1;
					}
				}
			}
			else
			{
				num = 0;
			}
			NRF_GPIO->PIN_CNF[KEY] = 0x00000002;
		}
		#ifdef __TFN108
		if(SampleCounter)
		{
			SampleCounter--;
			if(SampleCounter == 0)
			{
				SampleOverFlag = 1;
			}
		}
		if(Off_sec_Delay)
		{
			Off_sec_Delay--;
			if(Off_sec_Delay == 0)
				Off_sec_Over = 1;
		}
		OFF_Deal();
		if(!SampleOnFlag)
		{
			pah8011Data.close_flag = 1;
//			CloseHeartSample();
		}
		#endif
		
//	NRF_GPIO->PIN_CNF[IO_OFF_D] = 0x00000002;
		NRF_GPIO->PIN_CNF[IO_OFF_I] = 0x00000002;		
		NRF_POWER->EVENTS_POFWARN = 0;
	}
}
//FILE END
