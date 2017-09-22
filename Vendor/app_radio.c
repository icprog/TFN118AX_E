//app_radio.c
#include "nrf.h"
#include "app_init.h"
#include "app_var.h"
#include "stdbool.h"
#include "nrf_gpio.h"
#include "pah8011drv.h"
#include "nrf_nvmc.h"
#define 	RADIO_BUFFER_SIZE 					32
#define 	RADIO_OVER_TIME							10000

uint8_t radio_rcvok = 0;
uint8_t radio_sndok = 0;
uint8_t CycleSendEn = 0;											//周期发送秒标志
uint8_t CycleSendFlag = 0;										//周期性发送
uint8_t buf_ExtInfo[EXTINFO_LEN];							//扩展信息缓冲
uint8_t buf_Record[RECORD_LEN];								//记录缓冲
uint8_t para_Record[RECORD_LEN];							//用于读取内部参数

uint8_t RateErr = 0;													//异常
uint8_t Key = 0;															//按键
uint8_t Keybuf = 0;														//按键缓冲
uint8_t Shake = 0;														//振动
uint8_t LowVol = 0;														//低压
uint8_t DownCmdFlag = 0;											//收到下行指令标志
uint8_t DownRPDFlag = 0;											//下行指令RPD有效
uint8_t LowVolReportNum = 0;									//低压上报次数

uint8_t cur_Dir = 0xff;																	//射频当前的方向
uint8_t led_flag = 0;																		//LED点亮标志

extern uint8_t DeviceID[4];
extern uint8_t package[32];
extern uint8_t SensorValue;

void Osc_HFCLK(void);
void Osc_HFCLK_Off(void);
void RunPara(void);

extern pah8011Data_t pah8011Data;
extern uint64_t ucstamp;

/*******************************************************
函数名:RADIO开中断
参  数:无
返  回:无
*******************************************************/
void Radio_Int_On(void)
{
	//interrupt
	Osc_HFCLK();
	NRF_RADIO->INTENSET = (RADIO_INTENSET_END_Set << RADIO_INTENSET_END_Pos);
	NRF_RADIO->EVENTS_END = 0;
	NVIC_SetPriority(RADIO_IRQn,RADIO_PRIORITY);
	NVIC_ClearPendingIRQ(RADIO_IRQn);
	NVIC_EnableIRQ( RADIO_IRQn );
	radio_rcvok = 0;
}

void Radio_Int_Off(void)
{
	//interrupt
	NRF_RADIO->INTENCLR = 0xFFFFFFFF;
	NVIC_DisableIRQ( RADIO_IRQn );
	radio_rcvok = 0;
	Osc_HFCLK_Off();
}

uint8_t utl_GetSum(uint8_t *p,uint8_t l)
{
	uint8_t temp = 0,i;
	for(i = 0;i<l;i++)
	{
		temp ^= p[i];
	}
	return temp;
}
bool utl_SumCheck(uint8_t * p,uint8_t l)
{
	uint8_t sum,temp;
	uint8_t i;
	
	sum = *p;
	for(temp = 0,i = 0;i<l-1;i++)
	{
		temp ^= p[i+1];
	}
	if(temp == sum)
		return true;
	else
		return false;
}
void Delay_us(uint16_t us)
{
	uint8_t i;
	while(us --)
	{
		i = 1;
		while(i--);
	};
}
/*******************************************************
函数名:RADIO关中断
参  数:无
返  回:无
*******************************************************/
/*******************************************************
函数名:RADIO处理程序
参  数:无
返  回:无
*******************************************************/
void RADIO_IRQHandler(void)
{
	if(NRF_RADIO->EVENTS_END)//EVENTS_END
	{
		NRF_RADIO->EVENTS_END = 0;
		
		if(NRF_RADIO->STATE == RADIO_STATE_STATE_RxIdle)
		{	
			if(NRF_RADIO->CRCSTATUS == RADIO_CRCSTATUS_CRCSTATUS_CRCOk)
			{
				if(memcmp(DeviceID,package + TAG_ID_IDX,4)== 00)
				{
					radio_rcvok= 1;
				}
			}
		}
		else if(NRF_RADIO->STATE == RADIO_STATE_STATE_TxIdle)
		{
			radio_sndok= 1;
		}
	}
}
/****************************************
函数：清除射频缓冲区
输入：无
输出：无
****************************************/
void ClearRadioBuffer(void)
{
	uint8_t i;
	for(i = 0;i<RADIO_BUFFER_SIZE;i++)
	{
		package[i] = 0;
	}
}
/****************************************
函数：关闭射频
输入：无
输出：无
****************************************/
void radio_Disable(void)
{
	uint32_t ot;
	NRF_RADIO->EVENTS_DISABLED = 0;
	NRF_RADIO->TASKS_DISABLE = 1;
	while(NRF_RADIO->EVENTS_DISABLED == 0)
	{
		ot++;
		if(ot > RADIO_OVER_TIME)
			break;
	};
}
/****************************************
函数：射频功率选择
输入：无
输出：无
****************************************/
void radio_pwr(uint8_t s)
{
	switch(s)
	{
	case PARA_PWR_N30DBM:
		NRF_RADIO->TXPOWER = (RADIO_TXPOWER_TXPOWER_Neg30dBm << RADIO_TXPOWER_TXPOWER_Pos);
		break;
	case PARA_PWR_N20DBM:
		NRF_RADIO->TXPOWER = (RADIO_TXPOWER_TXPOWER_Neg20dBm << RADIO_TXPOWER_TXPOWER_Pos);
		break;
	case PARA_PWR_N16DBM:
		NRF_RADIO->TXPOWER = (RADIO_TXPOWER_TXPOWER_Neg16dBm << RADIO_TXPOWER_TXPOWER_Pos);
		break;
	case PARA_PWR_N12DBM:
		NRF_RADIO->TXPOWER = (RADIO_TXPOWER_TXPOWER_Neg12dBm << RADIO_TXPOWER_TXPOWER_Pos);
		break;
	case PARA_PWR_N8DBM:
		NRF_RADIO->TXPOWER = (RADIO_TXPOWER_TXPOWER_Neg8dBm << RADIO_TXPOWER_TXPOWER_Pos);
		break;
	case PARA_PWR_N4DBM:
		NRF_RADIO->TXPOWER = (RADIO_TXPOWER_TXPOWER_Neg4dBm << RADIO_TXPOWER_TXPOWER_Pos);
		break;
	case PARA_PWR_P0DBM:
		NRF_RADIO->TXPOWER = (RADIO_TXPOWER_TXPOWER_0dBm << RADIO_TXPOWER_TXPOWER_Pos);
		break;
	case PARA_PWR_P4DBM:
		NRF_RADIO->TXPOWER = (RADIO_TXPOWER_TXPOWER_Pos4dBm << RADIO_TXPOWER_TXPOWER_Pos);
		break;
	default:
		NRF_RADIO->TXPOWER = (RADIO_TXPOWER_TXPOWER_Pos4dBm << RADIO_TXPOWER_TXPOWER_Pos);
		break;
	}
}
/****************************************
函数：射频频道选择
输入：ch-频道选择(DATA-数据频道,CONFIG-配置频道)
			dr-方向选择(RX-接收，TX发送)
输出：无
****************************************/
void radio_select(uint8_t ch,uint8_t dr)
{
	uint32_t ot;
	if(ch == CHANNEL_DATA || ch == CHANNEL_CONFIG)
		 NRF_RADIO->FREQUENCY = (ch == CHANNEL_DATA?DEFAULT_CHANNEL_DATA:DEFAULT_CHANNEL_CONFIG);
	
	if(dr == RADIO_RX)
	{
		if(NRF_RADIO->STATE != RADIO_STATE_STATE_RxIdle)
		{
			NRF_RADIO->EVENTS_DISABLED = 0;
			NRF_RADIO->TASKS_DISABLE = 1;
			ot = 0;
			while(NRF_RADIO->EVENTS_DISABLED == 0)
			{
				ot++;
				if(ot > RADIO_OVER_TIME)
					break;
			};
		}
		cur_Dir = dr;
		NRF_RADIO->PACKETPTR = (uint32_t)package;
		NRF_RADIO->TASKS_RXEN = 1;
	}
	else if(dr == RADIO_TX)
	{
		cur_Dir = dr;
		radio_Disable();
	}
}
/****************************************
函数：射频发送触发
输入：无
输出：无
****************************************/
void radio_TriggerTx(void)
{
	uint32_t ot;
	if(NRF_RADIO->EVENTS_DISABLED == 1)
	{
		NRF_RADIO->EVENTS_DISABLED = 0;
	}
	radio_sndok = 0;
	NRF_RADIO->PACKETPTR = (uint32_t)package;
	NRF_RADIO->TASKS_TXEN = 1U;
	ot = 0;
	while(radio_sndok == 0)
	{
		ot++;
		if(ot > RADIO_OVER_TIME)
			break;
	};
}
/****************************************
函数：周期发送射频信息
输入：cmdflag - 是否携带指令
			winflag - 是否携带窗口
输出：无
****************************************/
extern uint8_t pah_start;
extern int16_t                mems_data[100 * 3];
//extern uint32_t hr_cnt;
//uint8_t hr_buf[2];
//uint32_t hr_addr = 0x30000;

void Radio_Period_Send(uint8_t cmdflag,uint8_t winflag)
{
//	uint8_t idx,page,tempLowVol = 0;
//	static uint8_t key_sendnum,err_sendnum,shake_sendnum;
	ClearRadioBuffer();
	if(pah_start)
	{
		package[1]=0;
		package[2]=0x81;
		package[3]=DeviceID[0];
		package[4]=DeviceID[1];
		package[5]=DeviceID[2];
		package[6]=DeviceID[3];
		package[7]=pah8011Data.hr;	//心率
		package[8]=pah8011Data.grade;//ppg信号等级
		package[9]=ucstamp>>8;
		package[10]=ucstamp;
		package[11]=mems_data[0]>>8;
		package[12]=mems_data[0];
		package[13]=mems_data[1]>>8;
		package[14]=mems_data[1];
		package[15]=mems_data[2]>>8;
		package[16]=mems_data[2];	
		package[17]=pah8011Data.hr_trust_level;	
		package[18]=pah8011Data.return_value;

//		if(pah8011Data.has_updated)
//		{
//			pah8011Data.has_updated = 0;
//			
//			hr_buf[0] = hr_cnt;
//			hr_buf[1] = pah8011Data.hr;
//			nrf_nvmc_write_bytes(hr_addr,hr_buf,2);
//			hr_addr = hr_addr +2;
//		}
		//XOR
		package[0] = utl_GetSum(package+1,RADIO_BUFFER_SIZE-1);

		radio_select(CHANNEL_DATA,RADIO_TX);
		radio_TriggerTx();
		//关闭发送模式
		radio_Disable();
	}
}
/****************************************
函数：射频解析
输入：无
输出：无
****************************************/
#define JOIN_ALL											0
#define JOIN_ONE											1
#define JOIN_NONE											2

#define FLAG_NODIR										0
#define FLAG_UP												1
#define FLAG_DOWN											2
void Radio_Deal(void)
{
//	static uint8_t wincount;
	static uint8_t cmd,page,idx;
	
	//标签接收/响应的数据
	CycleSendFlag = 0;
//	wincount ++;
//	if(wincount >= 10)
//	{
//		Radio_Period_Send(0,1);													//发送带接收窗口
//		wincount = 0;
//		radio_select(CHANNEL_CONFIG,RADIO_RX);
//		Delay_us(500);
//		radio_Disable();
//	}else{
//		if(CycleSendEn)//活动模式
//			Radio_Period_Send(0,0);													//常规发送
//	}
	Radio_Period_Send(0,0);													//常规发送
	if(radio_rcvok)
	{	
		//校验和校验
		if(utl_SumCheck(package,RADIO_BUFFER_SIZE) == false)
		{
			radio_rcvok = 0;
			return;
		}		
		if(package[DOWN_TYPE_IDX] == DOWN_WITH_CMD)
		{
			cmd = (package[EXTINFO_CMD_IDX] & EXTINFO_CMD_Msk) >> EXTINFO_CMD_Pos;
			page = (package[EXTINFO_FLSPG_IDX] & EXTINFO_FLSPG_Msk)>>EXTINFO_FLSPG_Pos;
			idx = 0;
			if(package[EXTINFO_RDVLD_IDX]& EXTINFO_RDVLD_Msk)
				idx = (package[EXTINFO_RDOS_IDX] & EXTINFO_RDOS_Msk)>>EXTINFO_RDOS_Pos;
			memcpy(buf_ExtInfo,package + EXTINFO_CMD_IDX,4);
			switch(cmd)
			{
				case EXTINFO_CMD_READ:
					if(Read_Para(page,idx,buf_ExtInfo+4) == true)
					{
						Radio_Period_Send(1,0);
					}
					break;
				case EXTINFO_CMD_WRITE:
					memcpy(buf_ExtInfo,package + EXTINFO_BASE_IDX,EXTINFO_LEN);
					if(Write_Para(page,idx,buf_ExtInfo+4) == true)
					{
						Radio_Period_Send(1,0);
						GetDeviceID();
					}
					break;
				case EXTINFO_CMD_ERASE:
					memset(buf_ExtInfo + 4,0xAA,RECORD_LEN);
					if(Erase_Para(page) == true)
					{
						Radio_Period_Send(1,0);
					}
					break;
				case EXTINFO_CMD_READCURRAM:
					memcpy(buf_ExtInfo + 4,para_Record,RECORD_LEN);
					Radio_Period_Send(1,0);
					break;
				case EXTINFO_CMD_WRITECURRAM:
					memcpy(buf_ExtInfo,package + EXTINFO_BASE_IDX,EXTINFO_LEN);
					memcpy(para_Record,buf_ExtInfo+4,RECORD_LEN);
					Radio_Period_Send(1,0);
					RunPara();
					break;
			}
		}
		radio_rcvok = 0;
	}
}
//FILE END
