/*******************************************************************************
** ��Ȩ:		  ������Ƽ����޹�˾
** �ļ���: 		i2c_ee.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.53
** ����: 		  djl
** ��������: 	2012-05-28
** ����:		  
** ����ļ�:	i2c_ee.h
** �޸���־��	2012-10-18
** ��Ȩ����   (C)2011-2012 ������Ƽ����޹�˾
*******************************************************************************/

/* ����ͷ�ļ� *****************************************************************/
#include "i2c.h"
#include "twi_master.h"
#include "sys.h"
/* �������� ------------------------------------------------------------------*/
/* �� ------------------------------------------------------------------------*/
/* ���� ----------------------------------------------------------------------*/


uint32_t DMATransFlag = 0;
uint16_t ReceiveCount = 0;
/* �������� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
#if 0
uint8_t i2c_device_read_byte(uint8_t address,uint8_t *p_read_byte,uint8_t length)  
{  
		bool i2c_trans_state=false;  
		i2c_trans_state=twi_master_transfer(i2c_device_address,&address,1,TWI_ISSUE_STOP);   //�豸��ַ+�Ĵ�����ַ
		while(i2c_trans_state==false);  
		i2c_trans_state=twi_master_transfer(i2c_device_address|TWI_READ_BIT,p_read_byte,length,TWI_ISSUE_STOP);//��ȡ����  
		while(i2c_trans_state==false);  
			
		return i2c_trans_state;  
}  

//I2C д�Ĵ���
char I2C_Write_Reg(uint8_t Slaveraddr,uint8_t addr, uint8_t data)
{  
		bool i2c_trans_state=false; 
		uint8_t buf[2];
		
		buf[0] = addr;
		buf[1] = data;
		i2c_trans_state = twi_master_transfer(Slaveraddr,buf,sizeof(buf),TWI_ISSUE_STOP);//д�ֽ�
		if(true == i2c_trans_state)
			return 0;
		else
			return 0xff; 
}  

#endif




//I2C д����
/*******************************************************************************
  * @��������	  I2C_SendDates(u8 Slaveraddr,u8* SendData,u8 DataLength)
  * @����˵��   I2C ���ͺ���
  * @�������   * SendData ���������׵�ַ DataLength�����ͳ��ȣ��Ĵ�����ַ+����  Slaveraddr���ӻ���ַ
  * @�������   0xFFʧ��  0�ɹ�
  * @���ز���   ��
*******************************************************************************/
char I2C_SendDates(u8 Slaveraddr,u8* SendData,u8 DataLength)
{
 		bool i2c_trans_state=false;   	
		i2c_trans_state  = twi_master_transfer(Slaveraddr,SendData,DataLength,TWI_ISSUE_STOP);  
		if(true == i2c_trans_state)
			return 0;
		else
			return 0xff;
}


/**
  * @brief  I2C_ReadNBytesFromAddr
  * @param  pointer for data emplacement, nb of bytes to read, address read start, slae address when read, slave address for write
  * @retval 0 OK -  0xFF KO
  */
/*******************************************************************************
  * @��������	  char I2C_ReadDatas(u8 Slaveraddr,u8 addr,u8 *dat, u8 nbytes)
  * @����˵��   I2C ���ͺ���
  * @�������   Slaveraddr �豸��ַ addr���Ĵ�����ַ-����ʼ��ַ  dat���ݻ����� nbytes�����ݸ���  
  * @�������   0xFFʧ��  0�ɹ�
  * @���ز���   ��
*******************************************************************************/
char I2C_ReadDatas(u8 Slaveraddr,u8 addr,u8 *dat, u8 nbytes)
{
	bool i2c_trans_state=false;  
	uint32_t timeout;
	i2c_trans_state=twi_master_transfer(Slaveraddr,&addr,1,TWI_ISSUE_STOP);   //�豸��ַ+�Ĵ�����ַ
	timeout = 0xFFFFFF;
	while((i2c_trans_state==false) && (--timeout));
	if (timeout == 0)
	{  
		return 0xff;
	}
	i2c_trans_state=twi_master_transfer((Slaveraddr|0x01),dat,nbytes,TWI_ISSUE_STOP);//��ȡ����  
	timeout = 0xFFFFFF;
	while((i2c_trans_state==false) && (--timeout));
	if (timeout == 0)
	{  
		return 0xff;
	}	
	if(true == i2c_trans_state)
		return ERR_NONE;
	else
		return 0xff;
}


