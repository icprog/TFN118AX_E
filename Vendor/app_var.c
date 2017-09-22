//app_var.c
#include "stdint.h"
#include "nrf_nvmc.h"
#include "app_var.h"
#include "stdbool.h"
#include "string.h"
#include "app_sensor.h"
#define ID_BEGIN												0x3EC00			//1KB
#define PARA_BEGIN 											0x3F000UL		//4KB,参数区

uint32_t dwtempAddr;
uint8_t DeviceID[4] = {0xFE,0xAA,0xAA,0xAA};
uint8_t ID_buf[4];
const uint8_t  para_default[RECORD_LEN] = 
{0xE7,0xE7,0xE7,0xE7,0xE7,//射频地址
2,//数据频道
7,//配置频道
16,//短号
0x61,//b7~4-发射功率0dbm,b3~2自动上报的携带信息(0保留1用户1 2用户2),b1~0-工作模式，0-保留模式，1-活动模式
0,0,0,0,//工厂ID
0x00,//心率传感
0x41,//b7-6,单位，分，b5-0，采样周期值，5分钟 //0x41 1e
0
};

extern uint8_t buf_Record[RECORD_LEN];							//记录缓冲
extern uint8_t para_Record[RECORD_LEN];							//用于读取内部参数
extern uint8_t CycleSendEn;
extern uint32_t SampleTime;
extern uint32_t SampleCounter;	
extern uint8_t SampleOverFlag;
extern uint16_t SampleOnTime;

/****************************************
函数：ID类别转换
输入：无
输出：无
****************************************/
uint8_t TransferID(uint32_t *id,uint8_t *p)
{
	uint8_t type;
	
	*id = ((p[0]&0x000000FF) << 24) 
						|((p[1]&0x000000FF) << 16) 
						|((p[2]&0x000000FF) << 8) 
						| p[3];
		
		if((*id) == ID_RESERVER0)
			type = ID_TYPE_ERROR;
		else if((*id) <= ID_TAP_REGION_Last)
			type = ID_TYPE_TAP;														//标签
		else if((*id) <= ID_RECEIVER_REGION_Last)
			type = ID_TYPE_RECEIVER;												//接收器/读写器
		else if((*id) <= ID_TRANSCEIVER_REGION_Last)
			type = ID_TYPE_TRANSCEIVER;										//发卡器(桌面)
		else if((*id) == ID_SELF_RP)
			type = ID_TYPE_SELF_RP;
		else if((*id) == ID_BC_TAP_RP)
			type = ID_TYPE_ALLTAP_RP;
		else if((*id) == ID_BC_TAP_NRP)
			type = ID_TYPE_ALLTAP_NRP;
		else if((*id) == ID_BROADCAST_READER_RP)
			type = ID_TYPE_ALLRECEIVER_RP;
		else
			type = ID_TYPE_ERROR;
		
		return type;
}
/****************************************
函数：读片上FLASH数据
输入：address-读取的首字节地址
			src-读取后存放的缓冲
			num_bytes-读取的字节数
输出：无
****************************************/
void sef_nvmc_read_bytes(uint32_t address, uint8_t * src, uint8_t num_bytes)
{
	uint8_t i;
	
	for(i = 0;i < num_bytes ;i++)
	{
		src[i] = (*(uint8_t *)(address + i));
	}
	//可能超出范围
}
/****************************************
函数：获取设备ID
输入：
输出：
****************************************/
void GetDeviceID(void)
{
	sef_nvmc_read_bytes(ID_BEGIN,DeviceID,4);
	uint8_t ram;
//	for(uint8_t i = 0; i<25 ; i++)
//	{
//		nrf_nvmc_page_erase(0x30000 + 0x400*i);
//	}
//	while(1);
//	sef_nvmc_read_bytes(0x30000,&ram,1);
//	if(ram!=0xff)
//		while(1);
//	for(uint8_t i = 0; i<25 ; i++)
//	{
//		nrf_nvmc_page_erase(0x30000 + 0x400*i);
//	}
}
/****************************************
函数：运行内部参数
输入：无
输出：无
****************************************/
void RunPara(void)
{
	uint16_t unit;
	uint8_t time;
	//工作模式：活动，或保留
	CycleSendEn = (para_Record[TAGP_WORKMODE_IDX] & TAGP_WORKMODE_Msk) >> TAGP_WORKMODE_Pos;
	
	if(CycleSendEn)
		radio_pwr((para_Record[TAGP_PWR_IDX] & TAGP_PWR_Msk) >> TAGP_PWR_Pos);
	else
		radio_pwr(PARA_PWR_N16DBM);
	
	switch((para_Record[TAGP_SSRUNIT_IDX] & TAGP_SSRUNIT_Msk) >> TAGP_SSRUNIT_Pos)
	{
		case 0x00:
			unit = 1;//秒
			break;
		case 0x01:
			unit = 60;//分
			break;
		default:
			unit = 3600;//时
			break;
	}
	time = (para_Record[TAGP_SSRVALUE_IDX] & TAGP_SSRVALUE_Msk) >> TAGP_SSRVALUE_Pos;
	SampleTime = (time * unit);//单位1s
	SampleTime <<= 3;//单位125ms
	if(SampleTime < SAMPLE_MIN_TIME)
		SampleTime = 0;
	SampleOverFlag = 1;//设置完，立即采样
	SampleCounter = SampleTime;
	SampleOnTime = SAMPLE_ON_TIME;			//8s持续采样
}
/****************************************
函数：获取有效参数
输入：无
输出：无
****************************************/
uint8_t Read_Para(uint8_t type,uint8_t idx,uint8_t *pbuf)
{
	uint8_t max_num = 0;
	dwtempAddr = 0xffffffff;
	
	switch(type)
	{
		case TYPE_PARA:dwtempAddr = RAM_PARA;max_num = 16;break;
		case TYPE_RESEVE:dwtempAddr = RAM_RESEVER;max_num = 16;break;
		case TYPE_USER1:dwtempAddr = RAM_USER1;max_num = 32;break;
		default:dwtempAddr = RAM_USER2;max_num = 32;break;
	}
	
	if(idx > max_num -1)
		return false;
	dwtempAddr += ((idx & 0x000000ff) << 4);
	sef_nvmc_read_bytes(dwtempAddr,pbuf,RECORD_LEN);
	return true;
}
/****************************************
函数：写记录
输入：无
输出：无
****************************************/
uint8_t Write_Para(uint8_t type,uint8_t idx,uint8_t *pbuf)
{
	uint8_t max_num = 0,i,j;
	static uint32_t dwTmp;
	dwtempAddr = 0xffffffff;
	
	switch(type)
	{
		case TYPE_PARA:dwtempAddr = RAM_PARA;max_num = 16;break;
		case TYPE_RESEVE:dwtempAddr = RAM_RESEVER;max_num = 16;break;
		case TYPE_USER1:dwtempAddr = RAM_USER1;max_num = 32;break;
		default:dwtempAddr = RAM_USER2;max_num = 32;break;
	}
	
	if(idx > max_num -1)
		return false;
	
	
	for(j = 0;j<max_num;j++)
	{
		sef_nvmc_read_bytes(dwtempAddr,buf_Record,RECORD_LEN);		
		for(i = 0;i<RECORD_LEN;i++)
		{
			if(buf_Record[i] != 0xff)
			break;
		}
		
		if(i == RECORD_LEN)
		{
			break;
		}
		dwtempAddr += RECORD_LEN;
	}
	
	if(j == max_num)//记录满
	{
		return false;
	}
	else
	{		
		if(type == 0)
		{
			memcpy(pbuf,"\xE7\xE7\xE7\xE7\xE7\x02\x07",7);
		}
		
		nrf_nvmc_write_bytes(dwtempAddr,pbuf,RECORD_LEN);
		if(type == TYPE_PARA)
		{
			sef_nvmc_read_bytes(dwtempAddr,para_Record,RECORD_LEN);//写成功，立即生效
			
			#ifdef _DEBUG
			nrf_nvmc_page_erase(ID_BEGIN);
			memset(DeviceID,0xff,4);
			#endif
			
			if(memcmp(ID_buf,"\xFF\xFF\xFF\xFF",4)==0)
			{
				if(TransferID(&dwTmp,para_Record+TAGP_DELIVERID_IDX) == ID_TYPE_TAP)
				{
					nrf_nvmc_write_bytes(ID_BEGIN,para_Record+TAGP_DELIVERID_IDX,4);
					sef_nvmc_read_bytes(ID_BEGIN,para_Record + TAGP_DELIVERID_IDX,4);//写成功，立即生效
				}
			}
			RunPara();
		}
		return true;
	}
}

/****************************************
函数：擦除记录
输入：无
输出：无
****************************************/
uint8_t Erase_Para(uint8_t type)
{
	dwtempAddr = 0xffffffff;
	switch(type)
	{
		case TYPE_PARA:dwtempAddr = RAM_PARA;break;
		case TYPE_RESEVE:dwtempAddr = RAM_RESEVER;break;
		case TYPE_USER1:dwtempAddr = RAM_USER1;break;
		default:dwtempAddr = RAM_USER2;break;
	}
	nrf_nvmc_page_erase(dwtempAddr);
	return true;
}
/****************************************
函数：获取有效参数
输入：无
输出：无
****************************************/
uint8_t Get_ValidPara(uint8_t type,uint8_t *pbuf,uint8_t isrun)
{
	uint8_t ret = 0xff,j,i;
	uint32_t addr = 0xffffffff;
	uint8_t max_num = 0;
	switch(type)
	{
		case TYPE_PARA:addr = RAM_PARA;max_num = 16;break;
		case TYPE_RESEVE:addr = RAM_RESEVER;max_num = 16;break;
		case TYPE_USER1:addr = RAM_USER1;max_num = 32;break;
		default:addr = RAM_USER2;max_num = 32;break;
	}
	for(j = 0;j<max_num;j++)
	{
		sef_nvmc_read_bytes(addr,buf_Record,RECORD_LEN);		
		for(i = 0;i<RECORD_LEN;i++)
		{
			if(buf_Record[i] != 0xff)
			break;
		}
		
		if(i == RECORD_LEN)
		{
			break;
		}
		addr += RECORD_LEN;
	}
	
	if(j == 0)//指定参数区，没有参数
	{
		ret = 0xff;
		if(type == 0)
			memcpy(pbuf,para_default,RECORD_LEN);
		else
			memset(pbuf,0xff,RECORD_LEN);
	}
	else
	{
		ret = j-1;
		addr -= RECORD_LEN;
		sef_nvmc_read_bytes(addr,pbuf,RECORD_LEN);
	}
	if(type == TYPE_PARA && isrun)
	{
		#ifdef _DEBUG
		memcpy(pbuf,para_default,RECORD_LEN);//测试用
		#endif
		RunPara();
	}
	return ret;
}

//FILE END
