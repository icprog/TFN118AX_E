/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 * @defgroup uart_example_pca10001_main main.c
 * @{
 * @ingroup uart_example_pca10001
 *
 * @brief UART Example Application main file.
 *
 * This file contains the source code for a sample application using UART.
 *
 */
#include "stdint.h"
#include "app_init.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "app_var.h"
#include "pah8011drv.h"
#include "Debug_log.h"

uint8_t volatile package[32];  ///< Received packet buffer
uint32_t DayCounter = DAY_TIME;								//天秒数
uint8_t DayOverFlag = 1;
run_level_t run_level = LEVEL_IDLE;							//运行级别
extern pah8011Data_t pah8011Data;

extern uint8_t LowVolReportNum;									//低压上报次数
extern uint8_t LowVol;														//低压
extern uint8_t led_flag;
extern uint8_t CycleSendFlag;
extern uint8_t SampleOverFlag;


void Radio_Deal(void);
void Radio_Int_On(void);
void Radio_Int_Off(void);
uint8_t Get_ValidPara(uint8_t type,uint8_t *pbuf,uint8_t isrun);
void Sensor_Deal(void);
void OFFaboutRate(void);
void Sensor_Value(void);
void CloseHeartSample(void);

extern uint8_t time_int_flag;
/*******************************************************
函数名:高频晶振初始化
参  数:无
返  回:无
*******************************************************/
void Osc_HFCLK(void)
{
	if((NRF_CLOCK->HFCLKSTAT & CLOCK_HFCLKSTAT_SRC_Xtal)!=1)
	{
		/* Start 16 MHz crystal oscillator */
		NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
		NRF_CLOCK->TASKS_HFCLKSTART = 1;

		/* Wait for the external oscillator to start up */
		while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) 
		{
		}
		while((NRF_CLOCK->HFCLKSTAT & CLOCK_HFCLKSTAT_STATE_Msk) == 0)
		{
		}
	}
}

extern uint8_t pah_start;
void Osc_HFCLK_Off(void)
{
	//当心率采集时不关闭外部时钟。
	if((NRF_CLOCK->HFCLKSTAT & CLOCK_HFCLKSTAT_SRC_Xtal)==1 && 0 == pah_start )
	{
		NRF_CLOCK->TASKS_HFCLKSTOP = 1;
	}
}
/*******************************************************
函数名：端口初始化
参  数:无
返  回:无
*******************************************************/
void port_Init(void)
{

}
/*******************************************************
函数名：端口初始化
参  数:无
返  回:无
功	能：1s调用一次
*******************************************************/
void PowerCheck(void)
{
	static uint8_t num = 0;
	if(DayOverFlag)
	{
		DayOverFlag = 0;
		LowVol = 0;
		LowVolReportNum=0;
		DayCounter = DAY_TIME;
	}
	
//	if(BatteryCheckOverFlag)
	{
//		BatteryCheckOverFlag = 0;
		if(NRF_POWER->EVENTS_POFWARN)
		{
			num ++;
		}
		else
			num = 0;
		
		if(num > 10)
		{
			LowVol = 1;//实时低压状态
			num = 0;
		}
	}
}
void EnableWDT(void)
{
	NRF_WDT->CRV = 143839;//4秒
	NRF_WDT->RREN = WDT_RREN_RR0_Enabled << WDT_RREN_RR0_Pos;
	NRF_WDT->TASKS_START = 1;
}
void FeedWDT(void)
{
	NRF_WDT ->RR[0] = 0x6E524635;
}


/**
 * @brief Function for application main entry. 
 * @return 0. int return type required by ANSI/ISO standard.
 */
int main(void)
{
  Osc_LFCLK();
	rtc_Init();
	power_Init();
	radio_Init();
//	timer_Init();
	GetDeviceID();
	Get_ValidPara(TYPE_PARA,para_Record,true);
//	EnableWDT();
	nrf_gpio_cfg_output(IO_OFF_D);
	nrf_gpio_pin_set(IO_OFF_D);
	pah8011_gpio_init();//PowerRdN
	while(1)
	{
//		debug_printf("WAKE_UP \n");
		
		if(led_flag)
		{
			nrf_gpio_cfg_output(LED);
			nrf_gpio_pin_clear(LED);
		}
		PowerCheck();
		#ifdef __TFN108
//		OFFaboutRate();
		if(1 == pah8011Data.close_flag)
		{
			pah8011Data.close_flag = 0;
			CloseHeartSample();
		}
		if(SampleOverFlag && CycleSendEn)//活动模式下检测心率
			Sensor_Deal();
		Sensor_Value();
		#endif
		if(CycleSendFlag)																//1秒周期发送或接收
		{
			Radio_Int_On();
			Radio_Deal();
			Radio_Int_Off();
		}
		if(led_flag)
		{
			led_flag = 0;
			NRF_GPIO->PIN_CNF[LED] = 0x00000002;
		}
		__WFI();
//		FeedWDT();
  }
}

/**
 *@}
 **/
