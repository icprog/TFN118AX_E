//app_var.c
#include "stdint.h"
#include "nrf_nvmc.h"
#include "app_var.h"
#include "stdbool.h"
#include "string.h"
#include "app_sensor.h"
#define ID_BEGIN												0x3EC00			//1KB
#define PARA_BEGIN 											0x3F000UL		//4KB,������

uint32_t dwtempAddr;
uint8_t DeviceID[4] = {0xFE,0xAA,0xAA,0xAA};
uint8_t ID_buf[4];
const uint8_t  para_default[RECORD_LEN] = 
{0xE7,0xE7,0xE7,0xE7,0xE7,//��Ƶ��ַ
2,//����Ƶ��
7,//����Ƶ��
16,//�̺�
0x61,//b7~4-���书��0dbm,b3~2�Զ��ϱ���Я����Ϣ(0����1�û�1 2�û�2),b1~0-����ģʽ��0-����ģʽ��1-�ģʽ
0,0,0,0,//����ID
0x00,//���ʴ���
0x41,//b7-6,��λ���֣�b5-0����������ֵ��5���� //0x41 1e
0
};

extern uint8_t buf_Record[RECORD_LEN];							//��¼����
extern uint8_t para_Record[RECORD_LEN];							//���ڶ�ȡ�ڲ�����
extern uint8_t CycleSendEn;
extern uint32_t SampleTime;
extern uint32_t SampleCounter;	
extern uint8_t SampleOverFlag;
extern uint16_t SampleOnTime;

/****************************************
������ID���ת��
���룺��
�������
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
			type = ID_TYPE_TAP;														//��ǩ
		else if((*id) <= ID_RECEIVER_REGION_Last)
			type = ID_TYPE_RECEIVER;												//������/��д��
		else if((*id) <= ID_TRANSCEIVER_REGION_Last)
			type = ID_TYPE_TRANSCEIVER;										//������(����)
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
��������Ƭ��FLASH����
���룺address-��ȡ�����ֽڵ�ַ
			src-��ȡ���ŵĻ���
			num_bytes-��ȡ���ֽ���
�������
****************************************/
void sef_nvmc_read_bytes(uint32_t address, uint8_t * src, uint8_t num_bytes)
{
	uint8_t i;
	
	for(i = 0;i < num_bytes ;i++)
	{
		src[i] = (*(uint8_t *)(address + i));
	}
	//���ܳ�����Χ
}
/****************************************
��������ȡ�豸ID
���룺
�����
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
�����������ڲ�����
���룺��
�������
****************************************/
void RunPara(void)
{
	uint16_t unit;
	uint8_t time;
	//����ģʽ���������
	CycleSendEn = (para_Record[TAGP_WORKMODE_IDX] & TAGP_WORKMODE_Msk) >> TAGP_WORKMODE_Pos;
	
	if(CycleSendEn)
		radio_pwr((para_Record[TAGP_PWR_IDX] & TAGP_PWR_Msk) >> TAGP_PWR_Pos);
	else
		radio_pwr(PARA_PWR_N16DBM);
	
	switch((para_Record[TAGP_SSRUNIT_IDX] & TAGP_SSRUNIT_Msk) >> TAGP_SSRUNIT_Pos)
	{
		case 0x00:
			unit = 1;//��
			break;
		case 0x01:
			unit = 60;//��
			break;
		default:
			unit = 3600;//ʱ
			break;
	}
	time = (para_Record[TAGP_SSRVALUE_IDX] & TAGP_SSRVALUE_Msk) >> TAGP_SSRVALUE_Pos;
	SampleTime = (time * unit);//��λ1s
	SampleTime <<= 3;//��λ125ms
	if(SampleTime < SAMPLE_MIN_TIME)
		SampleTime = 0;
	SampleOverFlag = 1;//�����꣬��������
	SampleCounter = SampleTime;
	SampleOnTime = SAMPLE_ON_TIME;			//8s��������
}
/****************************************
��������ȡ��Ч����
���룺��
�������
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
������д��¼
���룺��
�������
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
	
	if(j == max_num)//��¼��
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
			sef_nvmc_read_bytes(dwtempAddr,para_Record,RECORD_LEN);//д�ɹ���������Ч
			
			#ifdef _DEBUG
			nrf_nvmc_page_erase(ID_BEGIN);
			memset(DeviceID,0xff,4);
			#endif
			
			if(memcmp(ID_buf,"\xFF\xFF\xFF\xFF",4)==0)
			{
				if(TransferID(&dwTmp,para_Record+TAGP_DELIVERID_IDX) == ID_TYPE_TAP)
				{
					nrf_nvmc_write_bytes(ID_BEGIN,para_Record+TAGP_DELIVERID_IDX,4);
					sef_nvmc_read_bytes(ID_BEGIN,para_Record + TAGP_DELIVERID_IDX,4);//д�ɹ���������Ч
				}
			}
			RunPara();
		}
		return true;
	}
}

/****************************************
������������¼
���룺��
�������
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
��������ȡ��Ч����
���룺��
�������
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
	
	if(j == 0)//ָ����������û�в���
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
		memcpy(pbuf,para_default,RECORD_LEN);//������
		#endif
		RunPara();
	}
	return ret;
}

//FILE END
