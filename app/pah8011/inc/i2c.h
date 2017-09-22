#ifndef __I2C_EE_H
#define __I2C_EE_H



/* ����ͷ�ļ� ********************************************************/
#include "nrf.h"

/* �궨�� --------------------------------------------------------------------*/
#define u8    uint8_t
#define u16   uint16_t

//AS7000�豸��ַ
#define i2c_device_address 0x60  //0x30<<1

/* ��������------------------------------------------------------------------ */
char I2C_SendDates(u8 Slaveraddr,u8* SendData,u8 DataLength);
char I2C_ReadDatas(u8 Slaveraddr,u8 addr,u8 *dat, u8 nbytes);


#endif /* __I2C_EE_H */

/************ (C) COPYRIGHT 2011 Lierda **************�ļ�����*******/

