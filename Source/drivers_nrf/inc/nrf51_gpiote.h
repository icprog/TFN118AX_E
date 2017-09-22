#ifndef _NRF51_GPIOTE_H_
#define _NRF51_GPIOTE_H_

#include "nrf.h"

#include <stdint.h>

#define GPIOTE_CHANNEL_0	0
#define GPIOTE_CHANNEL_1	1
#define GPIOTE_CHANNEL_2	2
#define GPIOTE_CHANNEL_3	3
//typedef enum
//{
//	GPIOTE_CHANNEL_0 = 0,
//	GPIOTE_CHANNEL_1,
//	GPIOTE_CHANNEL_2,
//	GPIOTE_CHANNEL_3
//}gpiote_channel_t;

typedef void (*GPIOTE_EventHandler)(void);

typedef struct
{
	uint32_t mode;
	uint32_t psel;
	uint32_t polarity;
	uint32_t outinit;

}GPIOTE_InitType;

void GPIOTE_Deinit(uint32_t GPIOTE_Channel);

void GPIOTE_Init(uint32_t GPIOTE_Channel, GPIOTE_InitType *GPIOTE_InitStruct);

void GPIOTE_TaskOut(uint32_t GPIOTE_Channel);

void GPIOTE_SetEventHandler(uint32_t GPIOTE_Channel, GPIOTE_EventHandler cb);

#endif	/* _NRF51_GPIOTE_H_ */


