/*******************************************************************************
** 版权:		  利尔达科技有限公司
** 文件名: 		i2c_ee.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 4.53
** 作者: 		  djl
** 生成日期: 	2012-05-28
** 功能:		  
** 相关文件:	i2c_ee.h
** 修改日志：	2012-10-18
** 版权所有   (C)2011-2012 利尔达科技有限公司
*******************************************************************************/

/* 包含头文件 *****************************************************************/
#include "i2c.h"
#include "twi_master.h"
#include "sys.h"
/* 类型声明 ------------------------------------------------------------------*/
/* 宏 ------------------------------------------------------------------------*/
/* 变量 ----------------------------------------------------------------------*/


uint32_t DMATransFlag = 0;
uint16_t ReceiveCount = 0;
/* 函数声明 ------------------------------------------------------------------*/
/* 函数功能 ------------------------------------------------------------------*/
#if 0
uint8_t i2c_device_read_byte(uint8_t address,uint8_t *p_read_byte,uint8_t length)  
{  
		bool i2c_trans_state=false;  
		i2c_trans_state=twi_master_transfer(i2c_device_address,&address,1,TWI_ISSUE_STOP);   //设备地址+寄存器地址
		while(i2c_trans_state==false);  
		i2c_trans_state=twi_master_transfer(i2c_device_address|TWI_READ_BIT,p_read_byte,length,TWI_ISSUE_STOP);//读取数据  
		while(i2c_trans_state==false);  
			
		return i2c_trans_state;  
}  

//I2C 写寄存器
char I2C_Write_Reg(uint8_t Slaveraddr,uint8_t addr, uint8_t data)
{  
		bool i2c_trans_state=false; 
		uint8_t buf[2];
		
		buf[0] = addr;
		buf[1] = data;
		i2c_trans_state = twi_master_transfer(Slaveraddr,buf,sizeof(buf),TWI_ISSUE_STOP);//写字节
		if(true == i2c_trans_state)
			return 0;
		else
			return 0xff; 
}  

#endif




//I2C 写数据
/*******************************************************************************
  * @函数名称	  I2C_SendDates(u8 Slaveraddr,u8* SendData,u8 DataLength)
  * @函数说明   I2C 发送函数
  * @输入参数   * SendData 发送数据首地址 DataLength：发送长度，寄存器地址+数据  Slaveraddr：从机地址
  * @输出参数   0xFF失败  0成功
  * @返回参数   无
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
  * @函数名称	  char I2C_ReadDatas(u8 Slaveraddr,u8 addr,u8 *dat, u8 nbytes)
  * @函数说明   I2C 发送函数
  * @输入参数   Slaveraddr 设备地址 addr：寄存器地址-读起始地址  dat数据缓冲区 nbytes：数据个数  
  * @输出参数   0xFF失败  0成功
  * @返回参数   无
*******************************************************************************/
char I2C_ReadDatas(u8 Slaveraddr,u8 addr,u8 *dat, u8 nbytes)
{
	bool i2c_trans_state=false;  
	uint32_t timeout;
	i2c_trans_state=twi_master_transfer(Slaveraddr,&addr,1,TWI_ISSUE_STOP);   //设备地址+寄存器地址
	timeout = 0xFFFFFF;
	while((i2c_trans_state==false) && (--timeout));
	if (timeout == 0)
	{  
		return 0xff;
	}
	i2c_trans_state=twi_master_transfer((Slaveraddr|0x01),dat,nbytes,TWI_ISSUE_STOP);//读取数据  
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


