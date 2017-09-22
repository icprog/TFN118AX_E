#ifndef TIM_A
#define TIM_A
#include "nrf.h"

extern void Timer0_Init(uint8_t delayms);
extern void Timer1_Init(void);
extern void Timer2_Init(void);
void TIMER_DeInit(NRF_TIMER_Type *TIMERx);
void TIMER_Start(NRF_TIMER_Type *TIMERx);
void TIMER_Stop(NRF_TIMER_Type *TIMERx);
void TIMER_Clear(NRF_TIMER_Type *TIMERx);
#endif

